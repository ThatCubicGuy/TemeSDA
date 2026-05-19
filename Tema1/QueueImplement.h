#ifndef COLLECTIONS_GENERIC_QUEUE_IMPLEMENTATIONS
#define COLLECTIONS_GENERIC_QUEUE_IMPLEMENTATIONS

#include "QueueT.h"

#define QUEUE_IMPLEMENT(T)                                                  \
struct QueueCell_##T##_s {                                                  \
	QueueCell_##T Next;                                                     \
	T Value;                                                                \
};                                                                          \
typedef struct QueueEnumerator_##T##_s {                                    \
	struct IEnumerator_##T##_s _parent;                                     \
	Queue_##T _source;                                                      \
	QueueCell_##T _currentNode;                                             \
} *QueueEnumerator_##T;                                                     \
static bool QueueMoveNext_##T(IEnumerator_##T This)                         \
{                                                                           \
	QueueEnumerator_##T e = (QueueEnumerator_##T)This;                      \
	if (e->_currentNode == NULL) {                                          \
		if (e->_source->_start == NULL) return false;                       \
		e->_currentNode = e->_source->_start;                               \
		This->Current = e->_currentNode->Value;                             \
		return true;                                                        \
	}                                                                       \
	if ((e->_currentNode)->Next == NULL) return false;                      \
	e->_currentNode = e->_currentNode->Next;                                \
	This->Current = e->_currentNode->Value;                                 \
	return true;                                                            \
}                                                                           \
static void QueueReset_##T(IEnumerator_##T This)                            \
{                                                                           \
	((QueueEnumerator_##T)This)->_currentNode = NULL;                       \
	This->Current = default(T);                                             \
}                                                                           \
static void QueueDispose_##T(IEnumerator_##T This)                          \
{                                                                           \
	free(This);                                                             \
}                                                                           \
static IEnumerator_##T QueueGetEnumerator_##T(const IEnumerable_##T This)   \
{                                                                           \
	QueueEnumerator_##T allocinit(QueueEnumerator_##T, result) {            \
		._parent = (struct IEnumerator_##T##_s) {                           \
			.MoveNext = QueueMoveNext_##T,                                  \
			.Reset = QueueReset_##T,                                        \
			.Dispose = QueueDispose_##T                                     \
		},                                                                  \
		._currentNode = NULL,                                               \
		._source = (Queue_##T)This                                          \
	};                                                                      \
	return base(result);                                                    \
}                                                                           \
Queue_##T Queue_##T##__ctor()                           \
{                                                       \
	Queue_##T allocinit(Queue_##T, result) {            \
		._parent = (struct IEnumerable_##T##_s) {       \
			.GetEnumerator = QueueGetEnumerator_##T     \
		},                                              \
		.Count = 0,                                     \
		._start = NULL,                                 \
		._end = NULL                                    \
	};                                                  \
	return result;                                      \
}                                                       \
void Queue_##T##_Destroy(Queue_##T* source)             \
{                                                       \
	Queue_##T##_Clear(*source);                         \
	free(*source);                                      \
	*source = NULL;                                     \
}                                                       \
void Queue_##T##_Clear(Queue_##T source)                \
{                                                       \
	for (QueueCell_##T current = source->_start; source->Count > 0; current = source->_start) { \
		source->_start = source->_start->Next;          \
		free(current);                                  \
		source->Count -= 1;                             \
	}                                                   \
	source->_start = source->_end = NULL;               \
}                                                       \
void Queue_##T##_Enqueue(Queue_##T source, T item)      \
{                                                       \
	if (source->Count == 0) {                           \
		allocinit(QueueCell_##T, source->_end) {        \
			.Next = NULL,                               \
			.Value = item                               \
		};                                              \
		source->_start = source->_end;                  \
		source->Count = 1;                              \
		return;                                         \
	}                                                   \
	source->Count += 1;                                 \
	allocinit(QueueCell_##T, source->_end->Next) {      \
		.Next = NULL,                                   \
		.Value = item                                   \
	};                                                  \
	source->_end = source->_end->Next;                  \
}                                                       \
void Queue_##T##_EnqueueFirst(Queue_##T source, T item)	\
{                                                       \
	if (source->Count == 0) {                           \
		allocinit(QueueCell_##T, source->_start) {      \
			.Next = NULL,                               \
			.Value = item                               \
		};                                              \
		source->_end = source->_start;                  \
		source->Count = 1;                              \
		return;                                         \
	}                                                   \
	source->Count += 1;                                 \
	auto aux = source->_start;                          \
	allocinit(QueueCell_##T, source->_start) {          \
		.Next = aux,                                    \
		.Value = item                                   \
	};                                                  \
}                                                       \
T Queue_##T##_Peek(Queue_##T source)                    \
{                                                       \
	return source->_start->Value;                       \
}                                                       \
T Queue_##T##_Dequeue(Queue_##T source)                 \
{                                                       \
	source->Count -= 1;                                 \
	T result = source->_start->Value;                   \
	QueueCell_##T next = source->_start->Next;          \
	free(source->_start);                               \
	source->_start = next;                              \
	return result;                                      \
}                                                       \
bool Queue_##T##_TryPeek(Queue_##T source, T* out)      \
{                                                       \
	if (!source->_start) return false;                  \
	*out = Queue_##T##_Peek(source);                    \
	return true;                                        \
}                                                       \
bool Queue_##T##_TryDequeue(Queue_##T source, T* out)   \
{                                                       \
	if (!source->_start) return false;                  \
	*out = Queue_##T##_Dequeue(source);                 \
	return true;                                        \
}

#endif
