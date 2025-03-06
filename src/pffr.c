#include "pffr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char token[255];
    char c;
    long puta;
    long b;

 // マジックナンバー
    char magicNumber[13] = "doc Pffr 1\n\n\n";
    for(int i = 0; i < 13; i ++) {
        c = fgetc(pf);
        if(c != magicNumber[i]) {
            fputs("error: ナンバーコードが一致しません\n", stderr);
            exit(1);
        }
    }

 // アクセスプロセス
    char acsToken[4] = "acs ";
    for(int i = 0; i < 4; i ++) {
        c = fgetc(pf);
        if(c != acsToken[4]) {
            fputs("error: アクセスプロセスが破損しています\n", stderr);
            exit(1);
        }
    }
    for(int i = 0;  i < 3; i ++) {
        c = fgetc(pf);
        if(c == EOF) {
            fputs("error: アクセスプロセスが破損しています\n", stderr);
            exit(1);
        }
        token[i] = c;
    }

    fclose(pf);
}
