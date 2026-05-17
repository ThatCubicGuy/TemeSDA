#ifndef SDA_TRIE
#define SDA_TRIE
#include "EnumerableT.h"
#include "HashSetT.h"
#include "ListT.h"
#include "HeapT.h"
#include "String.h"

HASH_SET_DEFINE(string)
LIST_DEFINE(string)

typedef TAG(File) {
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

typedef TAG(RetrievalTree) {
    HashSet(File) FileRefs;
    TAG(RetrievalTree)* Children[26];
} *RetrievalTree;
RetrievalTree RetrievalTree__ctor();

void RT_Add(RetrievalTree source, string keyword, File file);

void RT_Del(RetrievalTree source, string keyword, File file);

IEnumerable(File) RT_GetRefs(RetrievalTree source, string keyword);

IEnumerable(File) RT_GetPrefix(RetrievalTree source, string prefix);

void RT_Destroy(RetrievalTree *source);

typedef TAG(NotFoundException) {
    IMPL(Exception);
} *NotFoundException;
NotFoundException NotFoundException__ctor(string msg);

#endif
