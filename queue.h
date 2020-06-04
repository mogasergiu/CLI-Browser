/*  */
#include "linked_list.h"

#ifndef QUEUE
#define QUEUE

#define HEAD(que) (((Queue*)que)->head)
#define TAIL(que) (((Queue*)que)->tail)
#define SIZE_QUE(que) (((Queue*)que)->size)
#define EMPTY_QUE(que) (HEAD(que) == NULL)

typedef struct {
    size_t size;
    SimpleList *head, *tail;
} Queue;

void* InitQ(size_t size);

/*
 * "copy" holds a custom function that properly copies the memory at which
 * some members of specific structs point to
 */
int PushQ(void *que, void *elem, void* (*copy)(void*, const void*, size_t));

/*
 * "copy" holds a custom function that properly copies the memory at which
 * some members of specific structs point to
 */
int PushPQ(void **que, void *elem, int (*getPriority)(void*, void*),
        void* (*copy)(void*, const void*, size_t), void(*freeFunc)(void*));

/*
 * function used to pop elements off the head of the queue;
 * "freeFunc" is supposed to hold a custom memory deallocation function;
 * "copy" holds a custom function that properly copies the memory at which
 * some members of specific structs point to
 */
int PopQ(void *que, void *elem, void (*freeFunc)(void*),
            void* (*copy)(void*, const void*, size_t));

// "freeFunc" is supposed to hold a custom memory deallocation function
void DelQ(void **que, void (*freeFunc)(void*));

#endif
