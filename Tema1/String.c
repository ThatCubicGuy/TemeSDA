#include "String.h"

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
	if (length < 0) return NULL;
	string result = alloc_array(char, length + 1);
	// Due to budget cuts, we do not have the budget for CHECK() nor allocinit()
	// at this point in the source code. Manual nullcheck it is.
	if (!result) {
		fprintf(stderr, "ERROR: Unable to allocate %d bytes of memory for a string!", length + 1);
		exit(1);
	}
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
