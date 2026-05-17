#ifdef USE_ARENA_ALLOC
#include "Keywords.h"
#include "System.h"

#define INITIAL_ARENA_SIZE 16777216UL // 16 MB
#define ARENA_DICTIONARY_SIZE 256UL // 256 B

typedef TAG(Allocation) {
    object location;
    size_t size;
    TAG(Allocation)* next;
} *Allocation;

TAG(ArenaStatic) {
    void* Start;
    size_t Tail;
    void* last_alloc;
    Allocation Allocs[ARENA_DICTIONARY_SIZE]; // 256 KB
} static Arena = {
    .Start = NULL,
    .Tail = 0,
    .last_alloc = NULL,
    .Allocs = {0},
};

static Allocation Allocs_Get(object obj)
{
    size_t index = object_GetHashCode(obj) & (ARENA_DICTIONARY_SIZE - 1);
    for (Allocation current = Arena.Allocs[index]; current->location; current = current->next) {
        if (current->location == obj) {
            return current;
        }
    }
    return NULL;
}

static void Allocs_Set(object obj, size_t size)
{
    size_t index = object_GetHashCode(obj) & (ARENA_DICTIONARY_SIZE - 1);
    Allocation* current = &Arena.Allocs[index];
    if (current) {
        for (current = &(*current)->next; current; current = &(*current)->next) {
            if ((*current)->location == obj) {
                if (size == 0) {
                    Allocation old = *current;
                    (*current) = (*current)->next;
                    free(old);
                }
                else (*current)->size = size;
                return;
            }
        }
        return;
    }
    Allocation node = malloc(sizeof(*node));
    *node = (TAG(Allocation)) {
        .location = obj,
        .size = size,
        .next = NULL,
    };
    *current = node;
}

static void RemoveNodes(Allocation alloc)
{
    if (alloc) {
        RemoveNodes(alloc->next);
        free((void*)alloc);
    }
}

void ARENA_INIT(void)
{
    DEBUG_WRITELINE("Called ARENA_INIT (Initial size: %zu B)", INITIAL_ARENA_SIZE);
    if (Arena.Start) {
        fprintf(stderr, "WARNING: Attempt to reinitialize arena!");
        return;
    }
    Arena.Start = malloc(INITIAL_ARENA_SIZE);
    if (!Arena.Start) {
        fprintf(stderr, "ERROR: Cannot allocate arena!");
        exit(EXIT_OUT_OF_MEMORY);
    }
}

void ARENA_DESTROY(void)
{
    DEBUG_WRITELINE("Called ARENA_DESTROY (Current arena size: %zu B)", Arena.Tail);
    if (!Arena.Start) {
        fprintf(stderr, "WARNING: Attempt to deallocate uninitialized arena!");
        return;
    }
    free(Arena.Start);
    Arena.Start = NULL;
    for (size_t i = 0; i < ARENA_DICTIONARY_SIZE; ++i) {
        RemoveNodes(Arena.Allocs[i]);
    }
}

void* memalloc_(size_t block_size)
{
    DEBUG_WRITELINE("Called memalloc with size %zu B. Current stack size: %zu B", block_size, Arena.Tail);
    if (block_size == 0) return Arena.last_alloc;
}

void* zeroalloc_(size_t block_size)
{
    DEBUG_WRITELINE("Called zeroalloc with size %zu B. Current stack size: %zu B", block_size, Arena.Tail);

}

void* memresize_(void* old_location, size_t new_size)
{
    DEBUG_WRITELINE("Called memresize on %p with new size %zu B. Current stack size: %zu B", old_location, new_size, Arena.Tail);

}

void memcopy_(void* dest, const void* source, size_t size)
{
    DEBUG_WRITELINE("Called memcopy from %p to %p with size %zu B. Current stack size: %zu B", source, dest, size, Arena.Tail);
    for (size_t i = 0; i < size; ++i) {
        ((byte*)dest)[i] = ((byte*)source)[i];
    }
}

void memfree_(void* location)
{
    Allocation* obj = Allocs_Find(location);
    if (!obj) return;

    *obj = default(Allocation);
}

void* memalloc_(size_t size)
{
    // Special functionality of memalloc_ - return last allocation if size is 0
    if (size == 0) return HeapTrace.alloc_count > 0 ? HeapTrace.allocs[HeapTrace.alloc_count - 1] : throwe(new(Exception)("No last allocation to get!"));
    DEBUG_ASSERT(HeapTrace.alloc_count >= 0 && HeapTrace.alloc_count < HEAPTRACE_SIZE - 1, "Heap alloc count: %zu\n", HeapTrace.alloc_count);
    HeapTrace.allocs[HeapTrace.alloc_count] = malloc(size);
    return HeapTrace.allocs[HeapTrace.alloc_count] ? HeapTrace.allocs[HeapTrace.alloc_count++] : throwe(new(OutOfMemoryException)("Not enough memory to allocate block of %d bytes", size));
}

void* zeroalloc_(size_t size)
{
    DEBUG_WRITELINE("Called zeroalloc with size %zu. Current heap allocations: %zu", size, HeapTrace.alloc_count);
    if (size == 0) throw new(Exception)("Cannot allocate block of size 0! (zeroalloc)");
    DEBUG_ASSERT(HeapTrace.alloc_count >= 0 && HeapTrace.alloc_count < HEAPTRACE_SIZE - 1, "Heap alloc count: %zu\n", HeapTrace.alloc_count);
    HeapTrace.allocs[HeapTrace.alloc_count] = calloc(1, size);
    return HeapTrace.allocs[HeapTrace.alloc_count] ? HeapTrace.allocs[HeapTrace.alloc_count++] : throwe(new(OutOfMemoryException)("Not enough memory to allocate block of %d bytes", size));
}

void* memresize_(void* obj, size_t new_size)
{
    DEBUG_WRITELINE("Called memresize with new size %zu. Current heap allocations: %zu", new_size, HeapTrace.alloc_count);
    if (new_size == 0) throw new(Exception)("Cannot allocate block of size 0! (memresize)");
    DEBUG_ASSERT(HeapTrace.alloc_count > 0 && HeapTrace.alloc_count < HEAPTRACE_SIZE, "Heap alloc count: %zu\n", HeapTrace.alloc_count);
    for (size_t i = 0; i < HeapTrace.alloc_count; ++i) {
        if (HeapTrace.allocs[i] == obj) {
            void* last_alloc = realloc(obj, new_size);
            return last_alloc ? HeapTrace.allocs[i] = last_alloc : throwe(new(OutOfMemoryException)("Not enough memory to reallocate to new block of %d bytes", new_size));
        }
    }
    throw new(Exception)("Reallocation of unallocated memory!");
}

void memfree_(void* obj)
{
    DEBUG_WRITELINE("Called memfree for location %p. Current heap allocations: %zu", obj, HeapTrace.alloc_count);
    if (obj == NULL) return;
    for (size_t i = 0; i < HeapTrace.alloc_count; ++i) {
        if (HeapTrace.allocs[i] == obj) {
            HeapTrace.alloc_count -= 1;
            free(HeapTrace.allocs[i]);
            for (size_t j = i; j < HeapTrace.alloc_count; ++j) {
                HeapTrace.allocs[j] = HeapTrace.allocs[j + 1];
            }
            HeapTrace.allocs[HeapTrace.alloc_count] = NULL;
            return;
        }
    }
    throw new(Exception)("Cannot free unallocated memory!");
}

void memcopy_(void* dest, const void* source, size_t size)
{
    DEBUG_WRITELINE("Called memcopy from %p to %p with size %zu. Current heap allocations: %zu", source, dest, size, HeapTrace.alloc_count);
    for (size_t i = 0; i < size; ++i) {
        ((byte*)dest)[i] = ((byte*)source)[i];
    }
}

#endif