#include "HashSetImplement.h"
// #undef throw
// #undef TAG
// #undef meminit
// #undef foreach
#include "lab.h"

TAG(HashSetEntry_Person) {
    HashSetEntry_Person Next;
    Person Value;
    size_t Hash;
};

typedef TAG(HashSetEnumerator_Person) {
    IMPL(IEnumerator_Person);
    HashSetEntry_Person _currentNode;
    int _currentIndex;
    HashSet_Person _set;
} *HashSetEnumerator_Person;

static bool HashSetMoveNext_Person(IEnumerator_Person This)
{
    HashSetEnumerator_Person e = (HashSetEnumerator_Person)This;
    while (!e->_currentNode && e->_currentIndex < 64) {
        e->_currentNode = e->_set->_items[e->_currentIndex];
        e->_currentIndex += 1;
    }
    if (e->_currentIndex == 64)
        return false;
    This->Current = e->_currentNode->Value;
    e->_currentNode = e->_currentNode->Next;
    return true;
}

static void HashSetReset_Person(IEnumerator_Person This)
{
    HashSetEnumerator_Person e = (HashSetEnumerator_Person)This;
    e->_currentIndex = 0;
    e->_currentNode = ((void *)0);
    This->Current = ((Person){0});
}

static void HashSetDispose_Person(IEnumerator_Person This)
{
    (((void)0), memfree_(This));
}

static IEnumerator_Person HashSetGetEnumerator_Person(IEnumerable_Person This)
{
    HashSetEnumerator_Person result = meminit(HashSetEnumerator_Person) {
        .MoveNext = HashSetMoveNext_Person,
        .Reset = HashSetReset_Person,
        .Dispose = HashSetDispose_Person,
        ._currentIndex = 0,
        ._currentNode = ((void *)0),
        ._set = (HashSet_Person)This
    };
    return (IEnumerator_Person)result;
}

void HashSet_Person_Add(HashSet_Person source, Person item)
{
    ThrowIfNull(source);
    HashSetEntry_Person node = meminit(HashSetEntry_Person) {
        .Hash = source->Comparer->GetHashCode(item),
        .Value = item,
        .Next = ((void *)0)
    };
    size_t index = node->Hash % 64;
    HashSetEntry_Person *bucket = &source->_items[index];
    while ((*bucket) && !(source->Comparer->Equals((*bucket)->Value, item))) {
        bucket = &(*bucket)->Next;
    }
    if (*bucket) {
        node->Next = (*bucket)->Next;
        (((void)0), memfree_(*bucket));
    } else
        source->Count += 1;
    *bucket = node;
}

HashSet_Person HashSet_Person__ctor(IEqualityComparer_Person comparer)
{
    ThrowIfNull(comparer);
    HashSet_Person result = meminit(HashSet_Person){
            .GetEnumerator = HashSetGetEnumerator_Person,
            .Comparer = comparer,
            .Count = 0,
    };
    return result;
}

HashSet_Person Enumerable_Person_ToHashSet(IEnumerable_Person source, IEqualityComparer_Person comparer)
{
    ThrowIfNull(source, comparer);
    HashSet_Person result = HashSet_Person__ctor(comparer);
    foreach (Person item in source) {
        HashSet_Person_Add(result, item);
    }
    return result;
}
