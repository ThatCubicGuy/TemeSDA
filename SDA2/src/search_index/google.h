#ifndef GOOGLE
#define GOOGLE
#include "retree.h"
#include "DoublyLinkedListT.h"
DOUBLY_LINKED_LIST_DEFINE(File)
typedef struct tag_System {
    DoublyLinkedList(File) Files;
    RetrievalTree Keywords;
} System[1];

/**
 * @brief Adds a file to the system.
 * If the file already exists, fails.
 * @returns True if the file was added, false if it already exists.
 */
bool Add(System sys, string id, int score, string keywords[], int keyword_count);
/**
 * @brief Removes a file from the system.
 * If the file does not exist, fails.
 * @returns True if the file was removed, false if it doesn't exist.
 */
bool Del(System sys, string id);

/**
 * @brief Adds a keyword to a file.
 * If the file does not exist, fails.
 * If the file already has the keyword, returns true.
 * @returns True if the file exists, false otherwise.
 */
bool AddKW(System sys, string id, string keyword);

/**
 * @brief Removes a keyword from a file.
 * Fails if the file does not exist.
 * @returns True if the file exists, false otherwise.
 */
bool DelKW(System sys, string id, string keyword);

/**
 * @brief Finds all files that match a keyword.
 * The results are sorted alphabetically.
 * @returns True if the file exists, false otherwise.
 */
bool Find(System sys, string keyword);

/**
 * @brief Finds the top k words associated with a keyword
 */
Heap(File) TopK(System sys, string keyword, int k);

#endif
