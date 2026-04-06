#include "STS.h"
#include "EnumerableImplement.h"
#include "DoublyLinkedListImplement.h"
#include "QueueImplement.h"
#include "StackImplement.h"

ENUMERABLE_IMPLEMENT(Unit)
ENUMERABLE_IMPLEMENT(Incident)
ENUMERABLE_IMPLEMENT(Intervention)

DOUBLY_LINKED_LIST_IMPLEMENT(Unit)
DOUBLY_LINKED_LIST_IMPLEMENT(Incident)
DOUBLY_LINKED_LIST_IMPLEMENT(Intervention)

QUEUE_IMPLEMENT(Unit)
QUEUE_IMPLEMENT(Incident)
STACK_IMPLEMENT(Intervention)
