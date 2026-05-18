#include "HashSetT.h"
#include "Keywords.h"
#include "retree.h"
#include "String.h"

#include <stdio.h>

TAG(IEqualityComparer(File)) FileComparer[1] = {(TAG(IEqualityComparer(File))) {
    .Equals = (bool(*)(File,File))object_ReferenceEquals,
    .GetHashCode = (size_t(*)(File))object_GetHashCode,
}};

static inline int idx(char c)
{
    return ('A' <= c && c <= 'Z') ? c - 'A' : c - 'a';
}

RetrievalTree RetrievalTree__ctor()
{
    RetrievalTree result = meminit(RetrievalTree) {0};
    return result;
}

File File__ctor(string id, int score, IEnumerable(string) keywords)
{
    ThrowIfNull(id, keywords);
    File result = memalloc(File);
    *result = init(File) {
        .ID = new(string)(id),
        .Score = score,
        .Keywords = Enumerable_string_ToHashSet(keywords, (void*)StringComparer.Ordinal.EqualityComparer)
    };
    return result;
}

void RT_Add(RetrievalTree source, string keyword, File file)
{
    ThrowIfNull(source, keyword, file);
    RetrievalTree current = source;
    fprintf(stderr, "Reading ");
    for (int i = 0; keyword[i]; ++i) {
        fprintf(stderr, "\033[2;%dm%c", i + 30, keyword[i]);
        if (!current->Children[idx(keyword[i])]) {
            current->Children[idx(keyword[i])] = new(RetrievalTree)();
        }
        current = current->Children[idx(keyword[i])];
    }
    fprintf(stderr, "\033[0m...\n");
    if (!current->FileRefs) current->FileRefs = new(HashSet(File))(FileComparer);
    fprintf(stderr, "Adding keyword \033[32m%s\033[0m to file \033[33m%s\033[0m\n", keyword, file->ID);
    HashSet_File_Add(current->FileRefs, file);
    fprintf(stderr, "New count: %d\n", current->FileRefs->Count);
}

void RT_Del(RetrievalTree source, string keyword, File file)
{
    ThrowIfNull(source, keyword, file);
    RetrievalTree current = source;
    for (int i = 0; keyword[i]; ++i) {
        if (!current->Children[idx(keyword[i])]) return;
        current = current->Children[idx(keyword[i])];
    }
    if (!current->FileRefs) return;
    fprintf(stderr, "Deleting keyword \033[32m%s\033[0m from file \033[33m%s\033[0m\n", keyword, file->ID);
    HashSet_File_Remove(current->FileRefs, file);
    fprintf(stderr, "New count: %d\n", current->FileRefs->Count);
    // Removing the keyword is simple! Just erase its hash set.
    if (current->FileRefs->Count == 0) {
        HashSet_File_Destroy(&current->FileRefs);
        fprintf(stderr, "HashSet for keyword \033[32m%s\033[0m has been \033[31mdestroyed.\033[0m\n", keyword);
    }
    // Proper container freeing will be done upon system destruction.
}

IEnumerable(File) RT_GetRefs(RetrievalTree source, string keyword)
{
    ThrowIfNull(source, keyword);
    RetrievalTree current = source;
    fprintf(stderr, "Getting all references for keyword \033[32m%s\033[0m\n", keyword);
    for (int i = 0; keyword[i]; ++i) {
        current = current->Children[idx(keyword[i])];
        if (!current) return Enumerable_File_Empty;
    }
    if (!current->FileRefs) return Enumerable_File_Empty;
    fprintf(stderr, "Ref count: %d\n", current->FileRefs->Count);
    return (IEnumerable(File))current->FileRefs;
}

static void FindFiles(RetrievalTree source, HashSet(File) result)
{
    if (!source) return;
    if (source->FileRefs) {
        foreach (File f in source->FileRefs) {
            HashSet_File_Add(result, f);
        }
    }
    for (int i = 0; i < 26; ++i) {
        FindFiles(source->Children[i], result);
    }
}

IEnumerable(File) RT_GetPrefix(RetrievalTree source, string prefix)
{
    ThrowIfNull(source, prefix);
    RetrievalTree current = source;
    fprintf(stderr, "Getting all references for prefix \033[35m%s\033[0m\n", prefix);
    for (int i = 0; prefix[i]; ++i) {
        current = current->Children[idx(prefix[i])];
        if (!current) return Enumerable_File_Empty;
    }
    HashSet(File) result = new(HashSet(File))(FileComparer);
    FindFiles(current, result);
    if (result->Count == 0) {
        // Memory management be damned...
        HashSet_File_Destroy(&result);
        return Enumerable_File_Empty;
    }
    return (IEnumerable(File))result;
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
