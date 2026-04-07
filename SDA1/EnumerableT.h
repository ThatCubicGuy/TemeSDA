#ifndef COLLECTIONS_GENERIC_ENUMERABLE
#define COLLECTIONS_GENERIC_ENUMERABLE
#include <stdbool.h>
#pragma region Define

/**
 * @brief Enumerates an IEnumerable<T> and sets var to
 * the current enumeration value before executing code.
 */
#define foreach(T, var, source, code...) do {               \
	IEnumerable_##T __src = (IEnumerable_##T)(source);      \
	IEnumerator_##T __e = (__src)->GetEnumerator(__src);    \
	while (__e->MoveNext(__e)) {                            \
		T var = __e->Current;                               \
		code;                                               \
	}                                                       \
	__e->Dispose(__e);                                      \
} while(0)
/**
 * @brief Returns a value inside a foreach. This is required
 * to properly dispose the enumerator.
 */
#define foreach_return(value) do { __e->Dispose(__e); return value; } while(0)

#define ENUMERABLE_DEFINE(T)                                                                            \
typedef struct IEnumerator_##T##_s {                                                                    \
	bool (*MoveNext)(struct IEnumerator_##T##_s* This);                                                 \
	void (*Reset)(struct IEnumerator_##T##_s* This);                                                    \
	void (*Dispose)(struct IEnumerator_##T##_s* This);                                                  \
	T Current;                                                                                          \
} *IEnumerator_##T;                                                                                     \
typedef const struct IEnumerable_##T##_s {                                                              \
	IEnumerator_##T (*GetEnumerator)(const struct IEnumerable_##T##_s* This);                           \
} *IEnumerable_##T;

#endif
