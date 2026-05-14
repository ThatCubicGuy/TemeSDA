#include "Keywords.h"
#include "trie.h"
#include "System/String.h"

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

struct tag_IEqualityComparer_File FileComparer[1] = {(struct tag_IEqualityComparer_File) {
    .Equals = (bool(*)(File,File))object_ReferenceEquals,
    .GetHashCode = (size_t(*)(File))object_GetHashCode,
}};

MultiWayTree MultiWayTree__ctor()
{
    return memalloc(MultiWayTree);
}

bool MWT_Add(MultiWayTree source, string keyword, File file)
{
    if (!keyword || !file) return false;
    MultiWayTree current = source;
    for (int i = 0; keyword[i]; ++i) {
        if (!current->Children[idx(keyword[i])]) {
            current->Children[idx(keyword[i])] = memalloc(MultiWayTree);
        }
        current = current->Children[idx(keyword[i])];
    }
    if (!current->FileRefs) current->FileRefs = new(HashSet(File))(FileComparer);
    if (HashSet_File_Contains(current->FileRefs, file)) {
        return false;
    }
    HashSet_File_Add(current->FileRefs, file);
    return true;
}

bool MWT_Del(MultiWayTree source, string keyword, File file)
{
    if (!keyword || !file) return false;
    MultiWayTree current = source;
    for (int i = 0; keyword[i]; ++i) {
        if (!current->Children[idx(keyword[i])]) {
            current->Children[idx(keyword[i])] = memalloc(MultiWayTree);
        }
        current = current->Children[idx(keyword[i])];
    }
    if (!current->FileRefs) return false;
    return HashSet_File_Remove(current->FileRefs, file);
}

IEnumerable(File) MWT_GetRefs(MultiWayTree source, string keyword)
{
    if (!keyword) return false;
    MultiWayTree current = source;
    for (int i = 0; keyword[i]; ++i) {
        if (!current->Children[idx(keyword[i])]) {
            current->Children[idx(keyword[i])] = memalloc(MultiWayTree);
        }
        current = current->Children[idx(keyword[i])];
    }
    if (!current->FileRefs) return NULL;
    return (IEnumerable(File))current->FileRefs;
}

void FindFiles(MultiWayTree source, List(File) result)
{
    if (!source) return;
    if (source->FileRefs) {
        foreach (File f in source->FileRefs) {
            List_File_Add(result, f);
        }
    }
    for (int i = 0; i < 26; ++i) {
        FindFiles(source->Children[i], result);
    }
}

int SortByScore(File left, File right)
{
    return left->Score - right->Score;
}

IEnumerable(File) MWT_GetPrefix(MultiWayTree source, string prefix)
{
    if (!prefix) return false;
    MultiWayTree current = source;
    for (int i = 0; prefix[i]; ++i) {
        if (!current->Children[idx(prefix[i])]) {
            current->Children[idx(prefix[i])] = memalloc(MultiWayTree);
        }
        current = current->Children[idx(prefix[i])];
    }
    List(File) result = new(List(File))(16);
    FindFiles(current, result);
    List_File_Sort(result, SortByScore);
    return (IEnumerable(File))result;
}
