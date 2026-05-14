#ifndef SDA_TRIE
#define SDA_TRIE
#include "EnumerableT.h"
#include "HashSetT.h"
#include "ListT.h"
#include "HeapT.h"

ENUMERABLE_DEFINE(string)
HASH_SET_DEFINE(string)
LIST_DEFINE(string)

typedef struct tag_File {
    string ID;
    int Score;
    HashSet(string) Keywords;
} *File;
File File__ctor(string id, int score, IEnumerable(string) keywords);

EQUALITY_COMPARER_DEFINE(File)
ENUMERABLE_DEFINE(File)
HASH_SET_DEFINE(File)
LIST_DEFINE(File)
HEAP_DEFINE(File)

typedef struct mwt_s *MultiWayTree;
struct mwt_s {
    HashSet(File) FileRefs;
    MultiWayTree Children[26];
};

MultiWayTree MultiWayTree__ctor();

bool MWT_Add(MultiWayTree source, string keyword, File file);

bool MWT_Del(MultiWayTree source, string keyword, File file);

IEnumerable(File) MWT_GetRefs(MultiWayTree source, string keyword);

Heap(File) MWT_GetPrefix(MultiWayTree source, string prefix);

#endif
