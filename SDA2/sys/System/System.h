#include "Keywords.h"
#include "String.h"
#include <stdlib.h>
#include <stdio.h>

#define EXIT_UNHANDLED_EXCEPTION 134
#define EXIT_ASSERT_FAILED 34
#define EXIT_OUT_OF_MEMORY 137

// Enable debug info
#define DEBUG 1
#define DEBUG_WRITELINE(FORMAT, ...) if(DEBUG) fprintf(stderr, FORMAT"\n",##__VA_ARGS__)
#define DEBUG_ASSERT(CONDITION, MESSAGE...) do { if(!(CONDITION)) { fprintf(stderr, "Assert failed: "#CONDITION"\n"); __VA_OPT__(fprintf(stderr, MESSAGE);) exit(EXIT_ASSERT_FAILED); } } while (0)
