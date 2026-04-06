#include "EnumerableT.h"
#include "String.h"

#ifndef STRING_INT_AGGREGATE_DEFINED
#define STRING_INT_AGGREGATE_DEFINED
ENUMERABLE_DEFINE_AGGREGATE(string, int)
#endif

const string string_Empty = "";

int string_Length(string source)
{
	if (source == NULL) return -1;
	int i = 0;
	while (source[i]) {
		i += 1;
	}
	return i;
}

#define c(PTR) ((char*)PTR)

string string__ctor(string other)
{
	int length = string_Length(other);
	string result = alloc_array(char, length + 1);
	for (int i = 0; i < length; ++i) {
		c(result)[i] = other[i];
	}
	c(result)[length] = 0;
	return result;
}

string string_Concat(string first, string second)
{
	int length = string_Length(first) + string_Length(second);
	string result = alloc_array(char, length + 1);
	int i = 0;
	while (first[i]) {
		c(result)[i] = first[i];
		i += 1;
	}
	for (int j = 0; second[i]; ++j) {
		c(result)[i] = second[j];
	}
	c(result)[length] = 0;
	return result;
}

#ifdef STRING_ENUMERABLE_DEFINED
#ifndef STRING_ENUMERABLE_IMPLEMENTED
#define STRING_ENUMERABLE_IMPLEMENTED
#include "EnumerableImplement.h"
ENUMERABLE_IMPLEMENT(string)
ENUMERABLE_IMPLEMENT_SELECT(string, string)
ENUMERABLE_IMPLEMENT_SELECTMANY(string, string)
ENUMERABLE_IMPLEMENT_AGGREGATE(string, string)
#endif
#endif
#ifdef STRING_INT_AGGREGATE_DEFINED
#ifndef STRING_INT_AGGREGATE_IMPLEMENTED
#define STRING_INT_AGGREGATE_IMPLEMENTED
#include "EnumerableImplement.h"
ENUMERABLE_IMPLEMENT_AGGREGATE(string, int)
#endif
#endif
