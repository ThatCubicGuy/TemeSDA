#include "Keywords.h"
#include "System.h"

void* _last_alloc = NULL;

void* _memalloc(size_t size)
{
    // Special functionality of _memalloc - return last allocation if size is 0
    if (size == 0) return _last_alloc;
    _last_alloc = malloc(size);
    return _last_alloc ? _last_alloc : (throw(new(OutOfMemoryException)("Not enough memory to allocate block of %d bytes", size)), NULL);
}

void* _zeroalloc(size_t size)
{
    _last_alloc = calloc(1, size);
    return _last_alloc ? _last_alloc : (throw(new(OutOfMemoryException)("Not enough memory to allocate block of %d bytes", size)), NULL);
}

void* _memresize(void* object, size_t new_size)
{
    _last_alloc = realloc(object, new_size);
    return _last_alloc ? _last_alloc : (throw(new(OutOfMemoryException)("Not enough memory to allocate block of %d bytes", new_size)), NULL);
}

void _memfree(void* object)
{
    free(object);
}
