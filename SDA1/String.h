#ifndef CUBE_STRING
#define CUBE_STRING
#include "Defines.h"

/**
 * @brief Represents the empty string.
 * This field is constant.
 */
extern const string string_Empty;

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
 * @brief Concatenates two strings and returns the result.
 * @return A new string.
 * @pure
 */
string string_Concat(string first, string second);

#endif
