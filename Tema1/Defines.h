#ifndef DEFINES
#define DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Calls the primary constructor of TYPE.
#define new(TYPE) TYPE##__ctor
// Allocates a single instance of REF_TYPE and returns its memory location.
#define alloc(REF_TYPE) ((REF_TYPE)malloc(sizeof(struct REF_TYPE##_s)))
// Allocates an array of CAPACITY elements of type ARRAY_TYPE and returns its location.
#define alloc_array(ARRAY_TYPE, CAPACITY) ((ARRAY_TYPE*)calloc(CAPACITY, sizeof(ARRAY_TYPE)))
// Initializes a potentially readonly reference type with a given initializer.
#define init(REF_TYPE, var) *(struct REF_TYPE##_s*)var = (struct REF_TYPE##_s)
// Allocates memory for a reference type and initializes its value with a given initializer.                                                                                                v Compile time so no NULL-deref errors :)
#define allocinit(REF_TYPE, var) var = alloc(REF_TYPE); if (!var) { fprintf(stderr, "ERROR: Failed allocation of %lu bytes for variable "#var" of type "#REF_TYPE" in file "__FILE__"\n", sizeof(*var)); exit(1); } init(REF_TYPE, var)
// Returns the type that this type directly inherits from.
#define base(REF_TYPE) (&(REF_TYPE)->_parent)
// Returns the default (zero initialized) value for the given type.
#define default(TYPE) ((TYPE){0})
// Default implementation of object equality.
#define equals(LEFT, RIGHT) (sizeof(typeof(LEFT)) == sizeof(typeof(RIGHT)) && ValueEquator(sizeof(typeof(LEFT)), &LEFT, &RIGHT))

typedef const void* object;
typedef const char* string;
typedef unsigned char byte;
// ^ lowercase instead of PascalCase since they're meant to be keywords

bool ValueEquator(size_t itemSize, object item1ref, object item2ref);

#endif
