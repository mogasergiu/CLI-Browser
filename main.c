/*  */
#include "browser.h"

#define MAX_LINE_SIZE 257

int openFiles(char **argv, int argc, FILE **fin, FILE **fout) {
    if (argc < 2) {
        return 0;
    }

    *fin = fopen(argv[1], "r");
    if (*fin == NULL) {
        return 0;
    }

    *fout = fopen(argv[2], "w");
    if (*fout == NULL) {
        fclose(*fin);
        return 0;
    }

    return 1;
}

int main(int argc, char **argv) {
    FILE *fin, *fout;
    if (!openFiles(argv, argc, &fin, &fout)) {
        return 0;
    }

    Browser *browser = BrowserInit();
    void *myTab = browser->tabs;
    int tabsNo = 0, bandwidth = 1024, which, steps, time;

    char *cmd = (char*)malloc(MAX_LINE_SIZE * sizeof(char));
    if (cmd == NULL) {
        freeBrowser(&browser);
        return 0;
    }

    while (fscanf(fin, "%s", cmd) == 1) {
        if (strcmp(cmd, "set_band") == 0) {
            fscanf(fin, "%d", &bandwidth);

        } else if (strcmp(cmd, "newtab") == 0) {
            newTab(&browser->tabs, &myTab);
            tabsNo++;

        } else if (strcmp(cmd, "deltab") == 0) {
            if (browser->tabs == myTab) {
                myTab = ((DoubleList*)browser->tabs)->prev;
            }
            PopTab(&browser->tabs);
            tabsNo--;

        } else if (strcmp(cmd, "change_tab") == 0) {
            fscanf(fin, "%d", &which);
            steps = tabsNo - which;
            myTab = findDL(browser->tabs, steps);

        } else if (strcmp(cmd, "print_open_tabs") == 0) {
            PrintDL(browser->tabs, printURL, tabsNo, fout);

        } else if (strcmp(cmd, "goto") == 0) {
            fscanf(fin, "%s", cmd);
            PushQ(browser->history, cmd, memcpy);
            gotoWebPage(myTab, cmd);
            updateDownloads(&browser->downloads, 1 * bandwidth);

        } else if (strcmp(cmd, "back") == 0) {
            if (!goBack(((DoubleList*)myTab)->info)) {
                fprintf(fout, "“can’t go back, no pages in stack”​\n");
            }

        } else if (strcmp(cmd, "forward") == 0) {
            if (!goForward(((DoubleList*)myTab)->info)) {
                fprintf(fout, "“can’t go forward, no pages in stack”​\n");
            }

        } else if (strcmp(cmd, "history") == 0) {
            printHistory(&browser->history, fout);

        } else if (strcmp(cmd, "del_history") == 0) {
            fscanf(fin, "%d", &which);
            deleteHistory(browser->history, which);

        } else if (strcmp(cmd, "list_dl") == 0) {
            printResources(((DoubleList*)myTab)->info, fout);

        } else if (strcmp(cmd, "downloads") == 0) {
            printDownloads(&browser->downloads, fout);

        } else if (strcmp(cmd, "download") == 0) {
            fscanf(fin, "%d", &which);
            Resource res = downloadResource(((DoubleList*)myTab)->info,
                which);
            if (strcmp(res.name, "failure") != 0) { // if resource is invalid
                PushPQ(&browser->downloads, &res, downloadPriority, memcpy,
                        free);  // free() is enough for a char array
            }

        } else if (strcmp(cmd, "wait") == 0) {
            fscanf(fin, "%d", &time);
            updateDownloads(&browser->downloads, time * bandwidth);
        }
    }

    freeBrowser(&browser);
    free(cmd);
    fclose(fin);
    fclose(fout);
    return 0;
}
