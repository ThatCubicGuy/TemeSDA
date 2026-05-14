#include "google.h"
#include "retree.h"
#include <stdio.h>

int main(void)
{
    System sys = {(struct tag_System){
        .Files = new(DoublyLinkedList(File))(),
        .Keywords = memalloc(RetrievalTree)
    }};
    return 1;
}
