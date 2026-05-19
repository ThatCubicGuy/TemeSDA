#include "google.h"
#include "retree.h"
#include <stdio.h>

bool Add(System sys, string id, int score, IEnumerable(string) keywords)
{
    foreach (File f in sys->Files) {
        if (StringComparer.Ordinal.Equals(f->ID, id)) return false;
    }
    File file = new(File)(id, score, keywords);
    DoublyLinkedList_File_Add(sys->Files, file);
    foreach (string kw in keywords) {
        RT_Add(sys->Keywords, kw, file);
    }
    return true;
}

bool Del(System sys, string id)
{
    foreach (File f in sys->Files) {
        if (StringComparer.Ordinal.Equals(f->ID, id)) {
            foreach (string kw in f->Keywords) {
                RT_Del(sys->Keywords, kw, f);
            }
            DoublyLinkedList_File_Remove(sys->Files, f);
            return true;
        }
    }
    return false;
}

bool AddKW(System sys, string id, string keyword)
{
    File target = NULL;
    foreach (File f in sys->Files) {
        if (StringComparer.Ordinal.Equals(f->ID, id)) {
            target = f;
            break;
        }
    }
    if (!target) return false;
    HashSet_string_Add(target->Keywords, new(string)(keyword));
    RT_Add(sys->Keywords, keyword, target);
    return true;
}

bool DelKW(System sys, string id, string keyword)
{
    File target = NULL;
    foreach (File f in sys->Files) {
        if (StringComparer.Ordinal.Equals(f->ID, id)) {
            target = f;
            break;
        }
    }
    if (!target) return false;
    HashSet_string_Remove(target->Keywords, keyword);
    RT_Del(sys->Keywords, keyword, target);
    return true;
}

static int AlphabeticalById(File left, File right)
{
    return StringComparer.Ordinal.Compare(left->ID, right->ID);
}

void Find(System sys, string keyword)
{
    // Use heap in find as well just because it's an easier way of storing items "in order"
    IEnumerable(File) refs = RT_GetRefs(sys->Keywords, keyword);
    Heap(File) files = new(Heap(File))(16, 4, AlphabeticalById);
    foreach (File f in refs) {
        Heap_File_Push(files, f);
        fprintf(stderr, "Heap count for \033[35m%s\033[0m: %d\n", keyword, files->Count);
    }
    if (files->Count == 0) {
        fprintf(stderr, "WARNING: Keyword %s has no references (Find)\n", keyword);
        fprintf(sys->output, "EMPTY\n");
        return;
    }
    fprintf(sys->output, "%d ", files->Count);
    File f;
    while (Heap_File_TryPop(files, &f)) fprintf(sys->output, "%s ", f->ID);
    fprintf(sys->output, "\n");
    Heap_File_Destroy(&files);
}

static int HighestScore(File left, File right)
{
    // Higher scores will be placed first because Heap<T> implements a minheap
    return right->Score == left->Score ? AlphabeticalById(left, right) : right->Score - left->Score;
    // And of COURSE we need unspecified secondary ordering
}

void TopK(System sys, string keyword, int count)
{
    IEnumerable(File) refs = RT_GetRefs(sys->Keywords, keyword);
    // Quaternary heaps are generally just better than binary or ternary heaps
    Heap(File) files = new(Heap(File))(16, 4, HighestScore);
    foreach (File f in refs) {
        Heap_File_Push(files, f);
        fprintf(stderr, "Heap count for \033[35m%s\033[0m: %d\n", keyword, files->Count);
    }
    if (files->Count == 0) {
        fprintf(stderr, "WARNING: Keyword %s has no references (TopK)\n", keyword);
        fprintf(sys->output, "EMPTY\n");
        return;
    }
    fprintf(sys->output, "%d ", (count > files->Count ? files->Count : count));
    File f;
    while ((count -= 1) >= 0 && Heap_File_TryPop(files, &f)) fprintf(sys->output, "%s ", f->ID);
    fprintf(sys->output, "\n");
    Heap_File_Destroy(&files);
}

static IEnumerable(string) GetKeywordsOfFile(File f)
{
    return (IEnumerable(string))f->Keywords;
}

void Print(System sys)
{
    List(string) keywords = NULL;
    do {
        // Use a hash set to get only distinct keywords from the
        // flattened collection of all keywords of every file in the system...
        HashSet(string) tmp = Enumerable_string_ToHashSet(
            Enumerable_File_SelectMany_string(
                (IEnumerable(File))sys->Files, GetKeywordsOfFile),
                (void*)StringComparer.Ordinal.EqualityComparer);
        // ...which I then shove in a list because I need them sorted.
        keywords = Enumerable_string_ToList((IEnumerable(string))tmp);
        HashSet_string_Destroy(&tmp);
    } while (0);
    if (keywords->Count == 0) {
        fprintf(stderr, "\033[1;33mWARNING\033[0m: No keywords in system\n");
        fprintf(sys->output, "EMPTY\n");
        return;
    }
    // I know it's a bit roundabout, but it works.
    List_string_Sort(keywords, StringComparer.Ordinal.Compare);
    foreach (string kw in keywords) {
        List(File) files = Enumerable_File_ToList(RT_GetRefs(sys->Keywords, kw));
        List_File_Sort(files, AlphabeticalById);
        fprintf(sys->output, "%s %d ", kw, files->Count);
        foreach (File f in files) {
            fprintf(sys->output, "%s ", f->ID);
        }
        List_File_Destroy(&files);
        fprintf(sys->output, "\n");
    }
}

void Prefix(System sys, string prefix)
{
    IEnumerable(File) refs = RT_GetPrefix(sys->Keywords, prefix);
    Heap(File) files = new(Heap(File))(16, 4, AlphabeticalById);
    foreach (File f in refs) {
        Heap_File_Push(files, f);
        fprintf(stderr, "Heap count for prefix \033[36m%s\033[0m: %d\n", prefix, files->Count);
    }
    if (files->Count == 0) {
        fprintf(stderr, "WARNING: Prefix %s has no references (TopK)\n", prefix);
        fprintf(sys->output, "EMPTY\n");
        return;
    }
    fprintf(sys->output, "%d ", files->Count);
    File f;
    while (Heap_File_TryPop(files, &f)) fprintf(sys->output, "%s ", f->ID);
    fprintf(sys->output, "\n");
    Heap_File_Destroy(&files);
}

#include "EnumerableImplement.h"
#include "DoublyLinkedListImplement.h"
#include "HashSetImplement.h"
#include "ListImplement.h"
#include "HeapImplement.h"

HASH_SET_IMPLEMENT(string)

ENUMERABLE_IMPLEMENT(File)
HASH_SET_IMPLEMENT(File)
LIST_IMPLEMENT(File)
HEAP_IMPLEMENT(File)
DOUBLY_LINKED_LIST_IMPLEMENT(File)
ENUMERABLE_IMPLEMENT_SELECT(File, string)
