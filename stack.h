/*  */
#include "linked_list.h"

#ifndef STACK
#define STACK

#define TOP(st) (((Stack*)st)->top)
#define EMPTY_ST(st) (TOP(st) == NULL)
#define SIZE_ST(st) (((Stack*)st)->size)

typedef struct {
    size_t size;
    SimpleList *top;
} Stack;

void* InitS(size_t size);

/*
 * "copy" holds a custom function that properly copies the memory at which
 * some members of specific structs point to
 */
int PushS(void *st, void *elem, void* (*copy)(void*, const void*, size_t));

/*
 * function used to pop elements off the top of the stack;
 * "freeFunc" is supposed to hold a custom memory deallocation function;
 * "copy" holds a custom function that properly copies the memory at which
 * some members of specific structs point to
 */
int PopS(void *st, void *elem, void (*freeFunc)(void*),
            void* (*copy)(void*, const void*, size_t));

// "freeFunc" is supposed to hold a custom memory deallocation function
void DelS(void **st, void (*freeFunc)(void*));

#endif
