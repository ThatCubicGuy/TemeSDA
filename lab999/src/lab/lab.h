#ifndef LAB_SDA
#define LAB_SDA

#include "Keywords.h"
#include "EnumerableT.h"
#include "HashSetT.h"

typedef TAG(Person) {
    string Name;
    int PhoneNumber;
} Person;

ENUMERABLE_DEFINE(Person)
EQUALITY_COMPARER_DEFINE(Person)
HASH_SET_DEFINE(Person)

#endif
