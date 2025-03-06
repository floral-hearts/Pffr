#include "pffr.h"

#include <stdio.h>
#include <stdlib.h>

void createDefaultFile(Pffr *pffr) {
    FILE *pf = fopen(pffr->path, "w");
    fputs("doc Pffr 1\n", pf);
    fputs("\n", pf);
    fputs("\n", pf);
    fputs("acs pgt 26\n", pf);
    fputs("\n", pf);
    fputs("\n", pf);
    fputs("pgc 1\n", pf);
    fputs("acs pgn 1 41\n", pf);
    fputs("\n", pf);
    fputs("\n", pf);
    fputs("pgs 210 297\n", pf);
    fputs("obj 0\n", pf);
    fclose(pf);
    pffr->info = setDefaultProcInfo();
    pffr->page = (ProcPage *)malloc(sizeof(ProcPage) * 1);
    pffr->page[0] = setDefaultProcPage();
    pffr->pageSize = 1;
    return;
}

void getFileContent(Pffr *pffr) {
    FILE *pf = fopen(pffr->path, "r");
    char token[255][10];
    int tokenSize = -1;
    int charSize = 0;
    char c;

 // マジックナンバー
    char magicNumber[13] = "doc Pffr 1\n\n\n";
    for(int i = 0; i < 13; i ++) {
        c = fgetc(pf);
        if(c != magicNumber[i]) {
            fputs("error: ナンバーコードが一致しません\n", stderr);
            exit(1);
        }
    }
}
