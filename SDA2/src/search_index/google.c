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

Heap(File) Find(System sys, string keyword)
{
    // Use heap in find as well just because it's an easier way of storing items "in order"
    Heap(File) files = new(Heap(File))(16, 4, AlphabeticalById);
    foreach (File f in RT_GetRefs(sys->Keywords, keyword)) {
        Heap_File_Push(files, f);
    }
    return files;
}

static int HighestScore(File left, File right)
{
    // Higher scores will be placed first because Heap<T> implements a minheap
    return right->Score - left->Score;
}

Heap(File) TopK(System sys, string keyword)
{
    // Quaternary heaps are generally just better than binary or ternary heaps
    Heap(File) files = new(Heap(File))(16, 4, HighestScore);
    foreach (File f in RT_GetRefs(sys->Keywords, keyword)) {
        Heap_File_Push(files, f);
    }
    return files;
}

static IEnumerable(string) GetKeywordsOfFile(File f)
{
    return (IEnumerable(string))f->Keywords;
}

void Print(System sys)
{
    List(string) keywords;
    do {
        // Use a hash set to get only distinct keywords from the
        // flattened collection of all keywords of every file in the system.
        HashSet(string) tmp = Enumerable_string_ToHashSet(
            Enumerable_File_SelectMany_string(
                (IEnumerable(File))sys->Files, GetKeywordsOfFile),
                (void*)StringComparer.Ordinal.EqualityComparer);
        keywords = Enumerable_string_ToList((IEnumerable(string))tmp);
        HashSet_string_Destroy(&tmp);
    } while (0);
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

#include "EnumerableImplement.h"
#include "DoublyLinkedListImplement.h"
#include "HashSetImplement.h"
#include "ListImplement.h"
#include "HeapImplement.h"

ENUMERABLE_IMPLEMENT(string)
HASH_SET_IMPLEMENT(string)
LIST_IMPLEMENT(string)

ENUMERABLE_IMPLEMENT(File)
HASH_SET_IMPLEMENT(File)
LIST_IMPLEMENT(File)
HEAP_IMPLEMENT(File)
DOUBLY_LINKED_LIST_IMPLEMENT(File)
ENUMERABLE_IMPLEMENT_SELECT(File, string)
