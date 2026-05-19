#include "Keywords.h"
#include "String.h"
#include <stdlib.h>
#include <stdio.h>

#define EXIT_UNHANDLED_EXCEPTION 134
#define EXIT_ASSERT_FAILED 34
#define EXIT_OUT_OF_MEMORY 137
#define EXIT_FAILFAST 1

#define DEBUG_WRITELINE(FORMAT, ...) if(DEBUG) fprintf(stderr, FORMAT"\n",##__VA_ARGS__)
#define DEBUG_ASSERT(CONDITION, ...) do { if(!(CONDITION)) { char buf[1024] = {0}; __VA_OPT__(sprintf(buf, "Assert failed: "#CONDITION __VA_ARGS__);) throw new(Exception)(buf); } } while (0)
#define DEBUG_MEMORYASSERT(PTR) if(!(PTR)) throw new(OutOfMemoryException)(sizeof(*(PTR)))
#define DEBUG_FAILFAST(MESSAGE...) do { fprintf(stderr, MESSAGE); exit(EXIT_FAILFAST); } while (0)
#define DEBUG_FAILFAST_IF(CONDITION, MESSAGE...) if (CONDITION) DEBUG_FAILFAST(MESSAGE)
#define DEBUG_FAILFAST_IF_NOT(CONDITION, MESSAGE...) if (!(CONDITION)) DEBUG_FAILFAST(MESSAGE)

#define STACKTRACE_SIZE 256
// New entry point for... reasons...
int start(int argc, char** argv);
