#ifndef COLLECTIONS_GENERIC_QUEUE
#define COLLECTIONS_GENERIC_QUEUE

#define QUEUE_DEFINE(T)                             \
typedef struct QueueCell_##T##_s *QueueCell_##T;    \
typedef struct Queue_##T##_s {                      \
	struct IEnumerable_##T##_s _parent;             \
	QueueCell_##T _start;                           \
	QueueCell_##T _end;                             \
	int Count;                                      \
} *Queue_##T;                                       \
Queue_##T Queue_##T##__ctor();                      \
/**                                                 \
 * @brief Frees up all memory used by a Queue<T>.   \
 * @param source Queue to free.                     \
 */                                                 \
void Queue_##T##_Destroy(Queue_##T* source);        \
/**                                                 \
 * @brief Removes all items from the Queue<T>.      \
 * @param source Queue to clear.                    \
 */                                                 \
void Queue_##T##_Clear(Queue_##T source);           \
/**                                                 \
 * @brief Adds an item to the end of the Queue<T>.  \
 * @param source Queue to append to.                \
 * @param item Item to append.                      \
 */                                                 \
void Queue_##T##_Enqueue(Queue_##T source, T item); \
/**                                                 \
 * @brief Adds an item to the start of the Queue<T>.\
 * @param source Queue to prepend to.               \
 * @param item Item to prepend.                     \
 */                                                 \
void Queue_##T##_EnqueueFirst(Queue_##T source, T item);\
/**                                                 \
 * @brief Queries the value of the first item in    \
 *  the Queue<T> and returns its value.             \
 * @param source Queue to peek.                     \
 * @return The value of the first item.             \
 */                                                 \
T Queue_##T##_Peek(Queue_##T source);               \
/**                                                 \
 * @brief Removes the first item from the Queue<T>  \
 * and returns its value.                           \
 * @param source Queue to dequeue from.             \
 * @return The value of the first item.             \
 */                                                 \
T Queue_##T##_Dequeue(Queue_##T source);            \
/**                                                 \
 * @brief Tries getting the fisrt item from the     \
 * Queue<T> without removing it and places it in    \
 * the address of out.                              \
 * @param source Queue to try peeking.              \
 * @param out Output of the peek.                   \
 * @return True if the peek succeeded,              \
 * false otherwise.                                 \
 */                                                 \
bool Queue_##T##_TryPeek(Queue_##T source, T* out); \
/**                                                 \
 * @brief Tries removing the fisrt item from the    \
 * Queue<T> and places it in out.                   \
 * @param source Queue to try dequeueing from.      \
 * @param out Output of the dequeue.                \
 * @return True if the dequeue succeeded,           \
 * false otherwise.                                 \
 */                                                 \
bool Queue_##T##_TryDequeue(Queue_##T source, T* out);

#endif
