#include "Defines.h"

#define b(PTR) ((byte*)PTR)

bool ValueEquator(size_t itemSize, object item1ref, object item2ref)
{
	for (size_t i = 0; i < itemSize; ++i) {
		if (b(item1ref)[i] != b(item2ref)[i]) return false;
	}
	return true;
}
