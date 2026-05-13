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
        .Keywords = Enumerable_string_ToList(keywords)
    };
    return result;
}

MultiWayTree MultiWayTree__ctor()
{
    return memalloc(MultiWayTree);
}

bool MWT_Add(MultiWayTree source, string keyword, File file)
{
    MultiWayTree current = source;
    if (Enumerable_File_Contains((IEnumerable(File))current->FileRefs, file)) {
        return false;
    }
    for (int i = 0; keyword[i]; ++i) {
        if (!current->Children[idx(keyword[i])]) {
            current->Children[idx(keyword[i])] = memalloc(MultiWayTree);
        }
        current = current->Children[idx(keyword[i])];
    }
    if (!current->FileRefs) current->FileRefs = new(List(File))(8);
    List_File_Add(current->FileRefs, file);
}

bool MWT_Del(MultiWayTree source, string keyword)
{
    
}

File MWT_Get(MultiWayTree source, string keyword)
{

}

List(File) MWT_GetPrefix(MultiWayTree source, string prefix)
{

}
