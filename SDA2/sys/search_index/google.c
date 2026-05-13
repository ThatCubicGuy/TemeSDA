#include "google.h"

bool Add(System sys, string id, int score, string keywords[], int keyword_count)
{
    List_File_Add(sys->Files, new(File)(id, score, keywords, keyword_count));
    MWT_Add(sys->SearchTree, sys->Files->Values[sys->Files->Count - 1], );
}

bool Del(System sys, string id);

bool AddKW(System sys, string keyword);

bool DelKW(System sys, string keyword);

bool Find(System sys, string keyword);

Heap(File) TopK(System sys, string keyword);

#include "ListImplement.h"
#include "HeapImplement.h"

LIST_IMPLEMENT(File)
HEAP_IMPLEMENT(File)
