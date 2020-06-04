/*  */
#include "browser.h"

void* WebPageDeepCopy(void *wp1, const void *wp2, size_t size) {
    if (size != sizeof(WebPage)) {
        return NULL;
    }

    WebPage *page1 = (WebPage*)wp1;
    WebPage *page2 = (WebPage*)wp2;

    if (page1->url == NULL) {
        page1->url = (char*)calloc(MAX_URL_SIZE, sizeof(char));
        if (page1->url == NULL) {
            return NULL;
        }
    }

    strcpy(page1->url, page2->url);

    if (page1->resources != NULL) {
        free(page1->resources);
    }

    page1->resources = get_page_resources(page2->url, &page1->num_res);

    return (void*)page1;
}

void printURL(void *t, FILE *f) {
    if (((Tab*)t)->current_page == NULL) {
        fprintf(f, "empty");
    } else {
        fprintf(f, "%s", ((Tab*)t)->current_page->url);
    }
}

void newTab(void **tabs, void **myTab) {
    void *newT = createTab();
    PushDL(tabs, newT, memcpy);
    *myTab = *tabs;
    free(newT);
}

void PopTab(void **tabs) {
    void *aux = malloc(sizeof(Tab));
    if (aux == NULL) {
        return;
    }

    PopDL(tabs, aux, freeTab, memcpy);
    free(aux);
}

void* createTab() {
    Tab *t = (Tab*)calloc(1, sizeof(Tab));
    if (t == NULL) {
        return NULL;
    }
    t->forward_stack = InitS(sizeof(WebPage));
    t->back_stack = InitS(sizeof(WebPage));

    return (void*)t;
}

void* getWebPage(char *url) {
    WebPage *wp = (WebPage*) calloc(1, sizeof(WebPage));
    if (wp == NULL) {
        return NULL;
    }

    wp->resources = get_page_resources(url, &wp->num_res);

    wp->url = (char*)calloc(MAX_URL_SIZE, sizeof(char));
    if (wp->url == NULL) {
        free(wp->resources);
        free(wp);
        return NULL;
    }

    strcpy(wp->url, url);

    return (void*)wp;
}

void gotoWebPage(void *t, char *url) {
    Tab *tab = (Tab*)((DoubleList*) t)->info;
    WebPage *wp = (WebPage*) getWebPage(url);

    if (tab->current_page != NULL) {
        PushS(tab->back_stack, tab->current_page, WebPageDeepCopy);
        freeWebPage(tab->current_page);
    }

    tab->current_page = wp;
}

void freeWebPage(void *wp) {
    if (wp != NULL) {
        WebPage *page = (WebPage*) wp;

        if (page->url != NULL) {
            free(page->url);
        }

        if (page->resources != NULL) {
            free(page->resources);
        }

        free(wp);
    }
}

void freeTab(void *t) {
    if (t != NULL) {
        Tab *tab = (Tab*)t;
        freeWebPage(tab->current_page);
        DelS(&tab->back_stack, freeWebPage);
        DelS(&tab->forward_stack, freeWebPage);
        free(t);
    }
}

void printDownloads(void **d, FILE *f) {
    void *auxDownloads = InitQ(sizeof(Resource));
    void *aux = malloc(sizeof(Resource));

    while (!EMPTY_QUE(*d)) {
        PopQ(*d, aux, free, memcpy);

        if (((Resource*)aux)->dimension ==
            ((Resource*)aux)->currently_downloaded) {
            fprintf(f, "[\"%s\" : completed]\n", ((Resource*)aux)->name);

        } else {
            fprintf(f, "[\"%s\" : %lu/%lu]\n",
                ((Resource*)aux)->name,
                ((Resource*)aux)->dimension -
                ((Resource*)aux)->currently_downloaded,
                ((Resource*)aux)->dimension);
        }
        PushQ(auxDownloads, aux, memcpy);
    }
    DelQ(d, free);
    free(aux);
    *d = auxDownloads;
}

int downloadPriority(void *res1, void *res2) {
    Resource *r1 = (Resource*)res1;
    Resource *r2 = (Resource*)res2;
    int dl1 = r1->dimension - r1->currently_downloaded;
    int dl2 = r2->dimension - r2->currently_downloaded;

    if (dl1 == 0) {
        return -1;
    }

    if (dl2 == 0) {
        return 1;
    }

    if (dl1 <= dl2) {
        return 1;
    } else {
        return -1;
    }
}

void updateDownloads(void **d, unsigned long downloaded) {
    if (EMPTY_QUE(*d)) {
        return;
    }

    Resource *r = (Resource*)calloc(1, sizeof(Resource));
    if (r == NULL) {
        return;
    }

    while (downloaded > 0) {
        PopQ(*d, r, free, memcpy);

        unsigned long remainder = r->dimension - r->currently_downloaded;
        if (remainder < downloaded) {
            /*
             * if the available downloadable data exceeds the remaining
             * data to be downloaded for the element at the head of the
             * "downloads" priority queue, then we substract that remainder,
             * put the element at the bottom and proceed to the next element
             */
            downloaded -= remainder;
            r->currently_downloaded = r->dimension;
            PushPQ(d, r, downloadPriority, memcpy, free);

        } else {
            /*
             * if the remaining data to be downloaded for the element at
             * the head of the "downloads" priority queue exceeds the
             * available downloadable data, then we just add it and don't
             * change anything else
             */
            r->currently_downloaded += downloaded;
            downloaded = 0;
            PushPQ(d, r, downloadPriority, memcpy, free);
        }
    }

    free(r);
}

void printResources(void *t, FILE *f) {
    if (t != NULL) {
        Tab *tab = (Tab*)t;

        if (tab->current_page != NULL) {
            WebPage *wp = tab->current_page;

            if (wp->resources != NULL) {
                Resource *r = wp->resources;

                int i;
                for (i = 0; i < wp->num_res; i++) {
                    fprintf(f, "[%d - \"%s\" : %lu]\n", i,
                        r[i].name, r[i].dimension);
                }
            }
        }
    }
}

Resource downloadResource(void *t, int whichResource) {
    if (((Tab*)t)->current_page != NULL) {
        Resource *r = ((Tab*)t)->current_page->resources;

        if (r != NULL || whichResource >= ((Tab*)t)->current_page->num_res) {
            return ((Tab*)t)->current_page->resources[whichResource];
        }
    }

    Resource fail;
    // in case the resource doesn't exist
    strcpy(fail.name, "failure");
    return fail;
}

int goBack(void *t) {
    Tab *tab = (Tab*)t;
    if (EMPTY_ST(tab->back_stack)) {
        return 0;
    }

    // push the current page into the forward_stack
    if (!PushS(tab->forward_stack, tab->current_page, WebPageDeepCopy)) {
        return 0;
    }

    // pop the top of the back_stack into the current page
    if (!PopS(tab->back_stack, tab->current_page, freeWebPage,
            WebPageDeepCopy)) {
        return 0;
    }

    return 1;
}

int goForward(void *t) {
    Tab *tab = (Tab*)t;
    if (EMPTY_ST(tab->forward_stack)) {
        return 0;
    }

    // push the current page into the back_stack
    if (!PushS(tab->back_stack, tab->current_page, WebPageDeepCopy)) {
        return 0;
    }

    // pop the top of the forward_stack into the current page
    if (!PopS(tab->forward_stack, tab->current_page, freeWebPage,
            WebPageDeepCopy)) {
        return 0;
    }

    return 1;
}

void printHistory(void **h, FILE *f) {
    void *auxHistory = InitQ(MAX_URL_SIZE * sizeof(char));
    char *entry = (char*)calloc(MAX_URL_SIZE, sizeof(char));
    if (entry == NULL) {
        DelQ(&auxHistory, free);
        return;
    }

    while (!EMPTY_QUE(*h)) {
        PopQ(*h, entry, free, memcpy);
        fprintf(f, "%s\n", entry);
        PushQ(auxHistory, entry, memcpy);
    }
    DelQ(h, free);
    free(entry);
    *h = auxHistory;
}

void deleteHistory(void *h, int n) {
    char *aux = (char*)calloc(MAX_URL_SIZE, sizeof(char));
    if (aux == NULL) {
        return;
    }

    if (n == 0) {
        while (!EMPTY_QUE(h)) {
            PopQ(h, aux, free, memcpy);
        }
    } else {
        while (!EMPTY_QUE(h) && n > 0) {
            PopQ(h, aux, free, memcpy);
            n--;
        }
    }

    free(aux);
}

Browser* BrowserInit() {
    Browser *b = calloc(1, sizeof(Browser));
    if (b == NULL) {
        return NULL;
    }

    b->history = InitQ(MAX_URL_SIZE * sizeof(char));
    if (b->history == NULL) {
        return NULL;
    }

    b->downloads = InitQ(sizeof(Resource));
    if (b->downloads == NULL) {
        DelQ(&b->history, free);
        return NULL;
    }

    b->tabs = InitDL(sizeof(Tab));
    if (b->tabs == NULL) {
        DelQ(&b->history, free);
        DelQ(&b->downloads, free);
        return NULL;
    }
    ((DoubleList*)b->tabs)->info = createTab();

    return b;
}

void freeBrowser(Browser **b) {
    if (*b != NULL) {
        if ((*b)->history != NULL) {
            DelQ(&(*b)->history, free);
        }

        if ((*b)->downloads != NULL) {
            DelQ(&(*b)->downloads, free);
        }

        if ((*b)->tabs != NULL) {
            DelDL(&(*b)->tabs, freeTab);
        }

        free(*b);
        *b = NULL;
    }
}
