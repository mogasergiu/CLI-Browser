/*  */
#include "stack.h"

void* InitS(size_t size) {
    Stack *st = (Stack*)calloc(1, sizeof(Stack));
    if (st == NULL) {
        return NULL;
    }

    st->size = size;

    return (void*)st;
}

int PushS(void *st, void *elem, void* (*copy)(void*, const void*, size_t)) {
    SimpleList *cell = (SimpleList*)calloc(1, sizeof(SimpleList));
    if (cell == NULL) {
        return 0;
    }

    cell->info = calloc(1, SIZE_ST(st));
    if (cell->info == NULL) {
        free(cell);
        return 0;
    }

    copy(cell->info, elem, SIZE_ST(st));
    cell->next = TOP(st);
    TOP(st) = cell;

    return 1;
}

int PopS(void *st, void *elem, void(*freeFunc)(void*),
    void* (*copy)(void*, const void * , size_t)) {
    if (EMPTY_ST(st)) {
        return 0;
    }

    copy(elem, TOP(st)->info, SIZE_ST(st));

    SimpleList * cell = TOP(st);
    TOP(st) = cell->next;
    freeFunc(cell->info);
    free(cell);
    return 1;
}

void DelS(void **st, void(*freeFunc)(void*)) {
    if (*st != NULL) {
        void *aux = malloc(SIZE_ST(*st));
        while (!EMPTY_ST(*st)) {
            /*
             * I directly passed the memcpy function as a parameter since
             * we don't care about keeping the popped element anyway
             */
            PopS(*st, aux, freeFunc, memcpy);
        }

        free(*st);
        free(aux);
        *st = NULL;
    }
}
