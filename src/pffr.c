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
    int c;
    long byte;
    long acsInfo = -1;
    long acsPage = -1;

 // マジックナンバー
    char magicNumber[13] = "doc Pffr 1\n\n\n";
    for(int i = 0; i < 13; i ++) {
        c = fgetc(pf);
        if(c != magicNumber[i]) {
            fputs("error: ナンバーコードが一致しません\n", stderr);
            exit(1);
        }
    }

 // アクセスプロセ
    while(1) {
        char acsToken[4] = "acs ";
        int next = 1;
        for(int i = 0; i < 4; i ++) {
            c = fgetc(pf);
            if(i == 1 && next && c == '\n') {
                break;
            } else if(i == 0 && c == '\n') {
                next = 1;
            } else if(c != acsToken[i]) {
                next = 0;
                fputs("error: acs proc: acs演算子\n", stderr);
                exit(1);
            } else {
                next = 0;
            }
        }
        if(next) {
            break;
        }
        for(int i = 0;  i < 3; i ++) {
            c = fgetc(pf);
            if(c == EOF) {
                fputs("error: acs proc: acs第1被演算子がありません\n", stderr);
                exit(1);
            }
            token[i] = c;
        }
        token[3] = '\0';
        if(fgetc(pf) != ' ') {
            fputs("error: アクセスプロセスが破損しています\n", stderr);
            exit(1);
        }
        if(!(strcmp(token, "inf") == 0 || strcmp(token, "pag") == 0)) {
            fputs("error: acs proc: acs第1被演算子\n", stderr);
            exit(1);
        }
        char acsStr[255] = { 0 };
        long acs;
        for(int i = 0; i < 255; i ++) {
            c = fgetc(pf);
            if('0' <= c && c <= '9') {
                acsStr[i] = (char)c;
            } else if(c == '\n') {
                break;
            } else {
                fputs("error: acs proc: acs第2被演算子\n", stderr);
                exit(1);
            }
        }
        acs = atol(acsStr);
        long *acsProc;
        acsProc = strcmp(token, "inf") == 0 ? &acsInfo : &acsPage;
        if(*acsProc != -1) {
            fputs("error: acs proc: アクセス項目が重複しています\n", stderr);
            exit(1);
        } else {
            *acsProc = acs;
        }
    }

    fclose(pf);
    return;
}
