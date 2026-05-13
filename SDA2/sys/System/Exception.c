#include "Keywords.h"
#include "System.h"

jmp_buf _finally_longjmp_buf = default(jmp_buf);
jmp_buf _finally_return_longjmp_buf = default(jmp_buf);

#include <stdio.h>
#define EXIT_OUTOFMEMORY 137

Exception Exception__ctor(string msg)
{
    Exception result = malloc(sizeof(struct _Exception));
    if (!result) {
        fprintf(stderr, "Error generating exception object of size %zu\n", sizeof(struct _Exception));
        exit(EXIT_OUTOFMEMORY);
    }
    result->Message = new(string)(msg);
    return result;
}

OutOfMemoryException OutOfMemoryException__ctor(string msg, size_t size)
{
    OutOfMemoryException result = (OutOfMemoryException)new(Exception)(string_Format(msg, size));
    result = realloc(result, sizeof(struct _OutOfMemoryException));
    // Can't really throw exceptions inside an exception constructor...
    if (!result) {
        fprintf(stderr, "Error generating exception object of size %zu\n", sizeof(struct _OutOfMemoryException));
        exit(EXIT_OUTOFMEMORY);
    }
    result->BlockSize = size;
    return result;
}
