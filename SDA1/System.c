#include "Defines.h"

#define b(PTR) ((byte*)PTR)

bool ValueEquator(size_t itemSize, object item1ref, object item2ref)
{
	for (size_t i = 0; i < itemSize; ++i) {
		if (b(item1ref)[i] != b(item2ref)[i]) return false;
	}
	return true;
}

int ValueComparer(size_t itemSize, object item1ref, object item2ref)
{
	for (size_t i = 0; i < itemSize; ++i) {
		if (b(item1ref)[i] != b(item2ref)[i]) {
			return -1 + 2 * (b(item1ref)[i] < b(item2ref)[i]);
		}
	}
	return 0;
}

bool ReferenceEquator(object item1, object item2)
{
	return item1 == item2;
}

inline void MemCopy(void* dest, const void* source, size_t nbytes)
{
	for (size_t i = 0; i < nbytes; ++i) {
		b(dest)[i] = b(source)[i];
	}
}

void MemCopyToNull(void* dest, const void* source)
{
	size_t i;
	for (i = 0; b(source)[i]; ++i) {
		b(dest)[i] = b(source)[i];
	}
	b(dest)[i] = '\0';
}
