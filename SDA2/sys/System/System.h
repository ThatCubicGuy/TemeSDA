#include "Keywords.h"
#include "String.h"
#include <stdlib.h>
#include <stdio.h>

#define EXIT_UNHANDLED_EXCEPTION 134
#define EXIT_ASSERT_FAILED 34
#define EXIT_OUT_OF_MEMORY 137

// Enable debug info
#define DEBUG 0
#define DEBUG_WRITELINE(FORMAT, ...) if(DEBUG) fprintf(stderr, FORMAT"\n",##__VA_ARGS__)
#define DEBUG_ASSERT(CONDITION, ...) do { if(!(CONDITION)) { fprintf(stderr, "Assert failed: "#CONDITION"\n"); __VA_OPT__(fprintf(stderr, __VA_ARGS__);) exit(EXIT_ASSERT_FAILED); } } while (0)

#undef DEBUG_ASSERT
#define DEBUG_ASSERT(CONDITION, ...) do { if(!(CONDITION)) { char buf[1024] = {0}; __VA_OPT__(sprintf(buf, "Assert failed: "#CONDITION __VA_ARGS__);) throw new(Exception)(buf); } } while (0)
