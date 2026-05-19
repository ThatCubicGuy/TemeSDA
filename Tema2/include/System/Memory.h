#ifndef C_DULL_MEMORY
#define C_DULL_MEMORY

#include "Keywords.h"

#define LIBC_MALLOC 0
#define HEAP_ALLOC 1
#define ARENA_ALLOC 2

#define ALLOC_TYPE HEAP_ALLOC

void* memalloc_(size_t block_size);
void* zeroalloc_(size_t block_size);
void* memresize_(void* old_location, size_t new_size);
void memcopy_(void* dest, const void* source, size_t size);
void memfree_(void* address);

#if DEBUG == true
#include <stdio.h>
#define memalloc(CLASS) (fprintf(stderr, "\033[35mAllocating\033[0m for %s (%s:%d)\n", #CLASS, __FILE__, __LINE__), (CLASS)memalloc_(sizeof(*(CLASS)0)))
#define arralloc(ARR_TYPE, LEN) (fprintf(stderr, "\033[35mArray allocating\033[0m for %s (%s:%d)\n", #ARR_TYPE, __FILE__, __LINE__), (typeof(ARR_TYPE)*)zeroalloc_(sizeof(ARR_TYPE) * LEN))
#define memcopy(DEST, SOURCE, SIZE) (fprintf(stderr, "\033[35mCopying\033[0m from %s to %s (%s:%d)\n", #SOURCE, #DEST, __FILE__, __LINE__), memcopy_(DEST, SOURCE, SIZE))
#define memresize(ARR, NEWLEN) (fprintf(stderr, "\033[35mResizing\033[0m %s to %d size (%s:%d)\n", #ARR, NEWLEN, __FILE__, __LINE__), (typeof(ARR))memresize_(ARR, sizeof(*(ARR)) * NEWLEN))
#define boxalloc(STRUCT) ((typeof(STRUCT)*)memalloc_(sizeof(STRUCT)))
#define memfree(PTR) (fprintf(stderr, "\033[35mFreeing\033[0m %s (%s:%d)\n", #PTR, __FILE__, __LINE__), memfree_(PTR))
#define meminit(CLASS) memalloc(CLASS); *(fprintf(stderr, "\033[35mInitializing\033[0m variable of type %s (%s:%d)", #CLASS, __FILE__, __LINE__), (TAG(CLASS)*)memalloc_(0)) = init(CLASS)
#else
#define memalloc(CLASS) ((CLASS)memalloc_(sizeof(*(CLASS)0)))
#define arralloc(ARR_TYPE, LEN) ((typeof(ARR_TYPE)*)zeroalloc_(sizeof(ARR_TYPE) * LEN))
#define memcopy(DEST, SOURCE, SIZE) memcopy_(DEST, SOURCE, SIZE)
#define memresize(ARR, NEWLEN) ((typeof(ARR))memresize_(ARR, sizeof(*(ARR)) * NEWLEN))
#define boxalloc(STRUCT) ((typeof(STRUCT)*)memalloc_(sizeof(STRUCT)))
#define memfree(PTR) memfree_(PTR)
#define meminit(CLASS) memalloc(CLASS); *(TAG(CLASS)*)memalloc_(0) = init(CLASS)
#endif
#define init(CLASS) (typeof(*(typeof(CLASS))0))

#endif