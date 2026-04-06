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
// Allocates memory for a reference type and initializes its value with a given initializer.
#define allocinit(REF_TYPE, var) var = alloc(REF_TYPE); init(REF_TYPE, var)
// Returns the type that this type directly inherits from.
#define base(REF_TYPE) (&(REF_TYPE)->_parent)
// Returns the default (zero initialized) value for the given type.
#define default(TYPE) ((TYPE){0})

typedef const void* object;
typedef const char* string;
typedef unsigned char byte;
// ^ lowercase instead of PascalCase since they're meant to be keywords

typedef void Action(object);
typedef bool Equator(object, object);
typedef int Comparer(object, object);

bool ValueEquator(size_t itemSize, object item1ref, object item2ref);

int ValueComparer(size_t itemSize, object item1ref, object item2ref);

bool ReferenceEquator(object item1, object item2);

void MemCopy(void* dest, const void* source, size_t nbytes);

void MemCopyToNull(void* dest, const void* source);

#endif
