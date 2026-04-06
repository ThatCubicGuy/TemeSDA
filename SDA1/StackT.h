#ifndef COLLECTIONS_GENERIC_STACK
#define COLLECTIONS_GENERIC_STACK

#define STACK_DEFINE(T)                             \
typedef struct StackCell_##T##_s *StackCell_##T;    \
typedef struct Stack_##T##_s {                      \
	struct IEnumerable_##T##_s _parent;             \
	StackCell_##T Start;                            \
	int Count;                                      \
} *Stack_##T;                                       \
Stack_##T Stack_##T##__ctor();                      \
/**                                                 \
 * @brief Frees up all memory used by a Stack<T>.   \
 * @param source Stack to free.                     \
 */                                                 \
void Stack_##T##_Destroy(Stack_##T* source);        \
/**                                                 \
 * @brief Removes all items from the Stack<T>.      \
 * @param source Stack to clear.                    \
 */                                                 \
void Stack_##T##_Clear(Stack_##T source);           \
/**                                                 \
 * @brief Adds an item to the top of the Stack<T>.  \
 * @param source Stack to push.                     \
 * @param item Item to push into the stack.         \
 */                                                 \
void Stack_##T##_Push(Stack_##T source, T item);    \
/**                                                 \
 * @brief Queries the value of the topmost item in  \
 *  the Stack<T> and returns its value.             \
 * @param source Stack to peek.                     \
 * @return The value of the topmost item.           \
 */                                                 \
T Stack_##T##_Peek(Stack_##T source);               \
/**                                                 \
 * @brief Removes the topmost item from the         \
 * Stack<T> and returns its value.                  \
 * @param source Stack to pop.                      \
 * @return The value of the topmost item.           \
 */                                                 \
T Stack_##T##_Pop(Stack_##T source);                \
/**                                                 \
 * @brief Tries getting the topmost item from the   \
 * Stack<T> without removing it and places it in    \
 * the address of out.                              \
 * @param source Stack to try peeking.              \
 * @param out Output of the peek.                   \
 * @return True if the peek succeeded,              \
 * false otherwise.                                 \
 */                                                 \
bool Stack_##T##_TryPeek(Stack_##T source, T* out); \
/**                                                 \
 * @brief Tries removing the topmost item from the  \
 * Stack<T> and places it in out.                   \
 * @param source Stack to try popping.              \
 * @param out Output of the pop.                    \
 * @return True if the pop succeeded,               \
 * false otherwise.                                 \
 */                                                 \
bool Stack_##T##_TryPop(Stack_##T source, T* out);

#endif
