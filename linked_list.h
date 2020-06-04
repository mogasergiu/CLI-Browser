/*  */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef LINKED_LIST
#define LINKED_LIST

typedef struct sll {
    struct sll *next;
    void *info;
} SimpleList;

typedef struct dll {
    struct dll *next, *prev;
    size_t size;
    void *info;
} DoubleList;

// initializes empty double linked list
void* InitDL(size_t size);

/*
 * pushes "elem" at the back of the double linked list;
 * "copy" holds a custom function that properly copies the memory at which
 * some members of specific structs point to
 */
int PushDL(void **dl, void *elem, void* (*copy)(void*, const void*, size_t));

/*
 * pops the element at the back of the double linked list into "elem"
 * "copy" holds a custom function that properly copies the memory at which
 * some members of specific structs point to;
 * "freeFunc" is a custom memory deallocation function
 */
int PopDL(void **dl, void *elem, void (*freeFunc)(void*),
            void* (*copy)(void*, const void*, size_t));

/*
 * Comletely deallocates the double linked list
 * "freeFunc" is a custom memory deallocation function
 */
void DelDL(void **dl, void (*freeFunc)(void*));

// returns element positioned "steps" behind the element in the list's back
void* findDL(void *dl, int steps);

/*
 * prints the elements of the list recursively;
 * "printFunc" is a custom printing function for specific elements
 */
void PrintDL(void *dl, void (*printFunc)(void*, FILE*), int no, FILE *fout);

#endif
