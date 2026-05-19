#ifndef C_DULL_EXCEPTIONS
#define C_DULL_EXCEPTIONS

#include "Keywords.h"

typedef struct tag_Exception {
    string Message;
    string File;
    int Line;
} *Exception;
Exception Exception__ctor(string message, ...);

typedef struct tag_OutOfMemoryException {
    struct tag_Exception;
    size_t BlockSize;
} *OutOfMemoryException;
OutOfMemoryException OutOfMemoryException__ctor(size_t block_size);

typedef struct tag_ArgumentException {
    struct tag_Exception;
    string ArgumentName;
} *ArgumentException;
ArgumentException ArgumentException__ctor(string message, string param_name);

typedef struct tag_ArgumentNullException {
    struct tag_ArgumentException;
} *ArgumentNullException;
ArgumentNullException ArgumentNullException__ctor(string param_name);
#define TO_NULL_CHECK(PARAM) if (!(PARAM)) throw new(ArgumentNullException)(nameof(PARAM));
// Automatically throw an exception if any of the given parameter is null.
#define ThrowIfNull(PARAMS...) do { FOREACH(TO_NULL_CHECK, PARAMS) } while (0)

typedef struct tag_ArgumentOutOfRangeException {
    struct tag_ArgumentException;
    int LowerBound, UpperBound;
} *ArgumentOutOfRangeException;
ArgumentOutOfRangeException ArgumentOutOfRangeException__ctor(string param_name, int lower_bound, int upper_bound);

typedef struct tag_IndexOutOfRangeException {
    struct tag_Exception;
    int Index, ArrayLength;
} *IndexOutOfRangeException;
IndexOutOfRangeException IndexOutOfRangeException__ctor(int index, int max_count);

enum StackTraceOperation {
    STACKTRACE_GET = -1,
    STACKTRACE_UNSET,
    STACKTRACE_SET
};
// Sets information about where the exception was thrown from.
extern void Exception_SetFrame(string filename, size_t line);

extern jmp_buf _finally_longjmp_buf;
extern jmp_buf _finally_return_longjmp_buf;
extern jmp_buf* StackTrace_Push(void);
extern jmp_buf* StackTrace_Peek(void);
extern jmp_buf* StackTrace_Pop(Exception);
extern Exception StackTrace_ThrownException(void);
extern void StackTrace_Rethrow(void);
extern int StackTrace_UpThrow(enum StackTraceOperation op);
extern int StackTrace_Finally(enum StackTraceOperation op);

#endif