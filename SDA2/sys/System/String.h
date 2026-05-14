#ifndef CUBE_STRING
#define CUBE_STRING
#include "Delegate.h"

/**
 * @brief Represents the empty string.
 * This field is constant.
 */
extern const string string_Empty;

EQUALITY_COMPARER_DEFINE(string)
COMPARER_DEFINE(string)
struct StringComparer_s {
    union {
        struct tag_IComparer_string Comparer[1];
        struct tag_IComparer_string;
    };
    union {
        struct tag_IEqualityComparer_string EqualityComparer[1];
        struct tag_IEqualityComparer_string;
    };
};
extern const struct StaticStringComparer_s {
    struct StringComparer_s Ordinal;
    struct StringComparer_s OrdinalIgnoreCase;
} StringComparer;

/**
 * @brief Generates the hash code for a string.
 * @returns A hash code that represents the given string.
 * @pure
 */
unsigned long string_HashCode(string source);
/**
 * @brief Copies a string and returns the result.
 * @return A new string with characters from the other.
 * @pure
 */
string string__ctor(string other);

/**
 * @brief Gets the length of the given string.
 * @returns An integer representing the
 * amount of characters in the string.
 * @pure
 */
int string_Length(string source);

/**
 * @brief Compares two strings and returns the result.
 * @returns A positive number if left > right,
 * a negative number if left < right, and zero if left == right.
 * @pure
 */
int string_Compare(string left, string right);

/**
 * @brief Concatenates two strings and returns the result.
 * @return A new string.
 * @pure
 */
string string_Concat(string first, string second);

/**
 * @brief Formats the given parameters into the format string and returns the result.
 * @return A new string.
 * @pure
 */
string string_Format(string format, ...);

#endif