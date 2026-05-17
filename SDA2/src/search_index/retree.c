#include "Keywords.h"
#include "retree.h"
#include "System/String.h"

#include <stdio.h>

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
        .Keywords = Enumerable_string_ToHashSet(keywords, (void*)StringComparer.Ordinal.EqualityComparer)
    };
    return result;
}

void RT_Add(RetrievalTree source, string keyword, File file)
{
    if (!keyword || !file) throw(new(Exception)("Keyword or file is null (RT_Add)"));
    fprintf(stderr, "Adding keyword \033[32m%s\033[0m to file \033[33m%s\033[0m\n", keyword, file->ID);
    RetrievalTree current = source;
    for (int i = 0; keyword[i]; ++i) {
        if (!current->Children[idx(keyword[i])]) {
            current->Children[idx(keyword[i])] = memalloc(RetrievalTree);
            *current->Children[idx(keyword[i])] = (struct tag_rt){0};
        }
        current = current->Children[idx(keyword[i])];
    }
    if (!current->FileRefs) current->FileRefs = new(HashSet(File))(FileComparer);
    HashSet_File_Add(current->FileRefs, file);
}

void RT_Del(RetrievalTree source, string keyword, File file)
{
    if (!keyword || !file) throw(new(Exception)("Keyword or file is null (RT_Del)"));
    fprintf(stderr, "Deleting keyword \033[32m%s\033[0m from file \033[33m%s\033[0m\n", keyword, file->ID);
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
    fprintf(stderr, "Getting all references for keyword \033[32m%s\033[0m\n", keyword);
    for (int i = 0; keyword[i]; ++i) {
        current = current->Children[idx(keyword[i])];
        if (!current) {
            fprintf(stderr, "Uh oh! [%c]\n", keyword[i]);
            return Enumerable_File_Empty;
        }
    }
    if (!current->FileRefs) throw(new(Exception)("ERR_NOT_TERMINAL_KW"));
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
    if (!prefix) throw(new(Exception)("Prefix is null (RT_GetPrefix)"));
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
