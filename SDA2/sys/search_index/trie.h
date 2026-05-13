#ifndef SDA_TRIE
#define SDA_TRIE
#include "EnumerableT.h"
#include "ListT.h"
#include "HeapT.h"

ENUMERABLE_DEFINE(string)
LIST_DEFINE(string)

typedef struct tag_File {
    string ID;
    int Score;
    List(string) Keywords;
} *File;
File File__ctor(string id, int score, IEnumerable(string) keywords);

ENUMERABLE_DEFINE(File)
LIST_DEFINE(File)
HEAP_DEFINE(File)

typedef struct mwt_s *MultiWayTree;
struct mwt_s {
    List(File) FileRefs;
    MultiWayTree Children[26];
};

MultiWayTree MultiWayTree__ctor();
bool MWT_Add(MultiWayTree source, string keyword, File file);
bool MWT_Del(MultiWayTree source, string keyword);
File MWT_Get(MultiWayTree source, string keyword);
List(File) MWT_GetPrefix(MultiWayTree source, string prefix);

#endif
