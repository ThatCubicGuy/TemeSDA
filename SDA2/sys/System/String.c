#include "String.h"

const string string_Empty = "";

static int StringComparerOrdinal(string left, string right)
{
    int len = string_Length(left);
    for (int i = 0; i <= len; ++i) {
        if (left[i] != right[i]) return left[i] - right[i];
    }
    return 0;
}
static inline char ToLower(char c)
{
    return ('A' <= c && c <= 'Z') ? c + ('a' - 'A') : c;
}
static int StringComparerOrdinalIgnoreCase(string left, string right)
{
    int len = string_Length(left);
    for (int i = 0; i <= len; ++i) {
        if (ToLower(left[i]) != ToLower(right[i])) {
            return ToLower(left[i]) - ToLower(right[i]);
        }
    }
    return 0;
}

const struct StringComparer_s StringComparer = {
    .Ordinal = StringComparerOrdinal,
    .OrdinalIgnoreCase = StringComparerOrdinalIgnoreCase
};

unsigned long string_HashCode(string source)
{
    unsigned long hashCode = 5381;
    int c = source[0];
    while ((c = *(source++))) {
        hashCode = ((hashCode << 5) + hashCode) + c;
    }
    return hashCode;
}

int string_Length(string source)
{
    if (source == NULL) return -1;
    int i = 0;
    while (source[i]) {
        i += 1;
    }
    return i;
}

string string__ctor(string other)
{
    int length = string_Length(other);
    string result = arralloc(char, length + 1);
    for (int i = 0; i < length; ++i) {
        ((char*)result)[i] = other[i];
    }
    ((char*)result)[length] = 0;
    return result;
}

int string_Compare(string left, string right)
{
    int len = string_Length(left);
    for (int i = 0; i <= len; ++i) {
        if (left[i] != right[i]) return left[i] - right[i];
    }
    return 0;
}

string string_Concat(string first, string second)
{
    int length = string_Length(first) + string_Length(second);
    string result = arralloc(char, length + 1);
    int i = 0;
    while (first[i]) {
        ((char*)result)[i] = first[i];
        i += 1;
    }
    for (int j = 0; second[i]; ++j) {
        ((char*)result)[i] = second[j];
    }
    ((char*)result)[length] = 0;
    return result;
}
