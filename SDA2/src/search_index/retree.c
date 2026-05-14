#include "Keywords.h"
#include "retree.h"
#include "System/String.h"

struct tag_IEqualityComparer_File FileComparer[1] = {(struct tag_IEqualityComparer_File) {
    .Equals = (bool(*)(File,File))object_ReferenceEquals,
    .GetHashCode = (size_t(*)(File))object_GetHashCode,
}};

static inline int idx(char c)
{
    return ('A' <= c && c <= 'Z') ? c - 'A' : c - 'a';
}

File File__ctor(string id, int score, IEnumerable(string) keywords)
{
    File result = memalloc(File);
    *result = init(File) {
        .ID = new(string)(id),
        .Score = score,
        .Keywords = Enumerable_string_ToHashSet(keywords)
    };
    return result;
}

void RT_Add(RetrievalTree source, string keyword, File file)
{
    if (!keyword || !file) throw(new(Exception)("Keyword or file is null (RT_Add)"));
    RetrievalTree current = source;
    for (int i = 0; keyword[i]; ++i) {
        if (!current->Children[idx(keyword[i])]) {
            current->Children[idx(keyword[i])] = memalloc(RetrievalTree);
        }
        current = current->Children[idx(keyword[i])];
    }
    HashSet_File_Add(current->FileRefs, file);
}

void RT_Del(RetrievalTree source, string keyword, File file)
{
    if (!keyword || !file) throw(new(Exception)("Keyword or file is null (RT_Del)"));
    RetrievalTree current = source;
    for (int i = 0; keyword[i]; ++i) {
        if (!current->Children[idx(keyword[i])]) throw(new(Exception)("Keyword does not exist (RT_Del)"));
        current = current->Children[idx(keyword[i])];
    }
    if (!current->FileRefs) throw(new(Exception)("Keyword is incomplete (RT_Del)"));
    HashSet_File_Remove(current->FileRefs, file);
    // Removing the keyword is simple! Just erase its hash set.
    if (current->FileRefs->Count == 0) {
        HashSet_File_Destroy(&current->FileRefs);
    }
    // Proper container freeing will be done upon system destruction.
}

IEnumerable(File) RT_GetRefs(RetrievalTree source, string keyword)
{
    if (!keyword) throw(new(Exception)("Keyword is null (RT_GetRefs)"));
    RetrievalTree current = source;
    for (int i = 0; keyword[i]; ++i) {
        if (!current->Children[idx(keyword[i])]) {
            current->Children[idx(keyword[i])] = memalloc(RetrievalTree);
        }
        current = current->Children[idx(keyword[i])];
    }
    if (!current->FileRefs) return NULL;
    return (IEnumerable(File))current->FileRefs;
}

static void FindFiles(RetrievalTree source, Heap(File) result)
{
    if (!source) return;
    if (source->FileRefs) {
        foreach (File f in source->FileRefs) {
            Heap_File_Push(result, f);
        }
    }
    for (int i = 0; i < 26; ++i) {
        FindFiles(source->Children[i], result);
    }
}

static int HighestScore(File left, File right)
{
    // Higher scores will be placed first because Heap<T> implements a minheap
    return right->Score - left->Score;
}

Heap(File) RT_GetPrefix(RetrievalTree source, string prefix)
{
    if (!prefix) return false;
    RetrievalTree current = source;
    for (int i = 0; prefix[i]; ++i) {
        if (!current->Children[idx(prefix[i])]) {
            current->Children[idx(prefix[i])] = memalloc(RetrievalTree);
        }
        current = current->Children[idx(prefix[i])];
    }
    // Quaternary heaps are generally just better than binary or ternary heaps
    Heap(File) result = new(Heap(File))(16, 4, HighestScore);
    FindFiles(current, result);
    return result;
}

void RemoveNodes(RetrievalTree start)
{
    if (!start) return;
    for (int i = 0; i < 26; ++i) {
        RemoveNodes(start->Children[i]);
    }
    memfree(start);
}

void RT_Destroy(RetrievalTree *source)
{
    RemoveNodes(*source);
    *source = NULL;
}
