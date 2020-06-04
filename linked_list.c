/*  */
#include "browser.h"

void* InitDL(size_t size) {
    DoubleList *dl = (DoubleList*)calloc(1, sizeof(DoubleList));
    if (dl == NULL) {
        return NULL;
    }

    dl->size = size;

    return (void*)dl;
}

int PushDL(void **dl, void *elem, void* (*copy)(void*, const void*, size_t)) {
    DoubleList *cell = (DoubleList*)calloc(1, sizeof(DoubleList));
    if (cell == NULL) {
        return 0;
    }

    DoubleList *tail = (DoubleList*)*dl;

    cell->info = calloc(1, tail->size);
    if (cell->info == NULL) {
        free(cell);
        return 0;
    }

    cell->size = tail->size;
    copy(cell->info, elem, tail->size);
    cell->prev = tail;
    tail->next = cell;
    *dl = cell;

    return 1;
}

int PopDL(void **dl, void *elem, void (*freeFunc)(void*),
        void* (*copy)(void*, const void*, size_t)) {
    if (*dl == NULL) {
        return 0;
    }

    DoubleList *tail = (DoubleList*)*dl;
    copy(elem, tail->info, tail->size);

    if (tail->prev == NULL) {
        *dl = NULL;
    } else {
        *dl = tail->prev;
        tail->prev->next = NULL;
    }

    freeFunc(tail->info);
    free(tail);

    return 1;
}

void DelDL(void **dl, void (*freeFunc)(void*)) {
    if (*dl != NULL) {
        void *aux = malloc(((DoubleList*)*dl)->size);
        while (PopDL(dl, aux, freeFunc, memcpy));

        free(aux);
    }
}

void* findDL(void *dl, int steps) {
    DoubleList *currElem = (DoubleList*)dl;
    while (currElem != NULL && steps > 0) {
        currElem = currElem->prev;
        steps--;
    }

    return (void*)currElem;
}

void PrintDL(void *dl, void(*printFunc)(void*, FILE*), int no, FILE *fout) {
    if (dl == NULL) {
        return;

    } else {
        if (((DoubleList*)dl)->prev != NULL)
            PrintDL(((DoubleList*)dl)->prev, printFunc, no - 1, fout);
        fprintf(fout, "(%d: ", no);
        printFunc(((DoubleList*)dl)->info, fout);
        fprintf(fout, ")\n");
    }
}
