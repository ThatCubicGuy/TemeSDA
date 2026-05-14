#include "google.h"
#include "retree.h"

bool Add(System sys, string id, int score, string keywords[], int keyword_count)
{
    foreach (File f in sys->Files) {
        if (f->ID == id) return false;
    }
    List(string) kws = new(List(string))(keyword_count);
    for (int i = 0; i < keyword_count; ++i) List_string_Add(kws, keywords[i]);
    File file = new(File)(id, score, (IEnumerable(string))kws);
    List_string_Destroy(&kws);
    DoublyLinkedList_File_Add(sys->Files, file);
    for (int i = 0; i < keyword_count; ++i) RT_Add(sys->Keywords, keywords[i], file);
    return true;
}

bool Del(System sys, string id)
{
    foreach (File f in sys->Files) {
        if (f->ID == id) {
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
        if (f->ID == id) {
            target = f;
            break;
        }
    }
    if (!target) return false;
    HashSet_string_Add(target->Keywords, keyword);
    return true;
}

bool DelKW(System sys, string id, string keyword)
{
    File target = NULL;
    foreach (File f in sys->Files) {
        if (f->ID == id) {
            target = f;
            break;
        }
    }
    if (!target) return false;
    HashSet_string_Remove(target->Keywords, keyword);
    return true;
}

bool Find(System sys, string keyword);

Heap(File) TopK(System sys, string keyword, int k);

#include "HashSetImplement.h"
#include "ListImplement.h"
#include "HeapImplement.h"

HASH_SET_IMPLEMENT(File)
LIST_IMPLEMENT(File)
HEAP_IMPLEMENT(File)
