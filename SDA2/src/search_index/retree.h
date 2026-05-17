#ifndef SDA_TRIE
#define SDA_TRIE
#include "EnumerableT.h"
#include "HashSetT.h"
#include "ListT.h"
#include "HeapT.h"
#include "Tuple.h"

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
ENUMERABLE_DEFINE_SELECT(File, string)

typedef struct tag_rt *RetrievalTree;
struct tag_rt {
    HashSet(File) FileRefs;
    RetrievalTree Children[26];
};

void RT_Add(RetrievalTree source, string keyword, File file);

void RT_Del(RetrievalTree source, string keyword, File file);

IEnumerable(File) RT_GetRefs(RetrievalTree source, string keyword);

IEnumerable(File) RT_GetPrefix(RetrievalTree source, string prefix);

void RT_Destroy(RetrievalTree *source);

typedef struct tag_NotFoundException {
    IMPL(Exception);
} *NotFoundException;
NotFoundException NotFoundException__ctor(string msg);

#endif
