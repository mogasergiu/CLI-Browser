/*  */
#include "queue.h"

void* InitQ(size_t size) {
    Queue *que = (Queue*) calloc(1, sizeof(Queue));
    if (que == NULL) {
        return NULL;
    }

    que->size = size;

    return (void * ) que;
}

int PushQ(void *que, void *elem, void* (*copy)(void*, const void*, size_t)) {
    SimpleList *cell = (SimpleList*)calloc(1, sizeof(SimpleList));
    if (cell == NULL) {
        return 0;
    }

    cell->info = calloc(1, SIZE_QUE(que));
    if (cell->info == NULL) {
        free(cell);
        return 0;
    }

    copy(cell->info, elem, SIZE_QUE(que));
    cell->next = NULL;

    if (EMPTY_QUE(que)) {
        TAIL(que) = HEAD(que) = cell;

    } else {
        TAIL(que)->next = cell;
        TAIL(que) = cell;
    }

    return 1;
}

int PushPQ(void **que, void *elem, int (*getPriority)(void*, void*),
    void* (*copy)(void*, const void* , size_t), void(*freeFunc)(void*)) {
    SimpleList *cell = (SimpleList*) calloc(1, sizeof(SimpleList));
    if (cell == NULL) {
        return 0;
    }

    cell->info = calloc(1, SIZE_QUE(*que));
    if (cell->info == NULL) {
        free(cell);
        return 0;
    }

    copy(cell->info, elem, SIZE_QUE(*que));

    if (EMPTY_QUE(*que)) {
        TAIL(*que) = HEAD(*que) = cell;

    } else {
        void *newQ = InitQ(SIZE_QUE(*que));
        void *aux = calloc(1, SIZE_QUE(*que));
        if (aux == NULL) {
            freeFunc(cell->info);
            free(cell);
            DelQ(&newQ, free);
        }

        while (!EMPTY_QUE(*que)) {
            if (getPriority(cell->info, HEAD(*que)->info) >= 0) {
                break;

            } else {
                PopQ(*que, aux, freeFunc, copy);
                PushQ(newQ, aux, copy);
            }
        }

        PushQ(newQ, cell->info, copy);
        while (!EMPTY_QUE(*que)) {
            PopQ(*que, aux, freeFunc, copy);
            PushQ(newQ, aux, copy);
        }

        DelQ(que, freeFunc);
        freeFunc(cell->info);
        free(cell);
        free(aux);
        *que = newQ;
    }

    return 1;
}

int PopQ(void *que, void *elem, void(*freeFunc)(void*),
    void* (*copy)(void*, const void*, size_t)) {
    if (EMPTY_QUE(que)) {
        return 0;
    }

    copy(elem, HEAD(que)->info, SIZE_QUE(que));

    SimpleList * cell = HEAD(que);
    HEAD(que) = cell->next;
    freeFunc(cell->info);
    free(cell);

    if (HEAD(que) == NULL) {
        TAIL(que) = NULL;
    }

    return 1;
}

void DelQ(void **que, void(*freeFunc)(void*)) {
    if (*que != NULL) {
        void *aux = malloc(SIZE_QUE(*que));

        while (!EMPTY_QUE(*que)) {
            /*
             * I directly passed the memcpy function as a parameter since
             * we don't care about keeping the popped element anyway
             */
            PopQ(*que, aux, freeFunc, memcpy);
        }

        free(*que);
        free(aux);
        *que = NULL;
    }
}
