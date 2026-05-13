#include "System.h"
#define b(PTR) ((byte*)PTR)

bool object_Equals(size_t size, object left, object right)
{
    for (size_t i = 0; i < size; ++i) {
        if (b(left)[i] != b(right)[i]) return false;
    }

    return true;
}
