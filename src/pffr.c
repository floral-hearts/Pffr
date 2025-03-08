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
    pffr->version = 1;
    pffr->info = setDefaultProcInfo();
    pffr->page = (ProcPage *)malloc(sizeof(ProcPage) * 1);
    pffr->page[0] = setDefaultProcPage();
    pffr->pageSize = 1;
    return;
}

void getFileContent(Pffr *pffr) {
    FILE *pf = fopen(pffr->path, "r");
    char ope[4];
    char str255[255];
    int c;
    long byte;
    long acsInfo = -1;
    long acsPage = -1;

 // マジックナンバー
    ope[0] = '\0';
    fgetToken(pf, ope, 4);
    if(strcmp(ope, "doc") != 0) {
        fputs("error: break operator\n", stderr);
        exit(1);
    }
    fgetToken(pf, str255, 255);
    if(strcmp(ope, "Pffr") != 0) {
        fputs("error: break 1st doc operand\n", stderr);
        exit(1);
    }
    fgetToken(pf, str255, 255);
    if((pffr.version = atoi(str255)) != 1) {
        fputs("error: cannnot open file\n", pf);
        exit(1);
    }
    while(strcmp(str255, "\n\n\n") == 0) {
        fgetToken(pf, str255, 255);
    }

 // アクセスプロセス
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

 // インフォメーションプロセス
    pffr.info = setDefaultProcInfo();
    if(acsInfo != -1) {
        if(fseek(pf, acsInfo, SEEK_SET) != 0) {
            fputs("error: acs proc: infプロセスのアクセス先が存在しません\n", stderr);
            exit(1);
        }
        for(int i = 0; i < 3; i ++) {
            c = fgetc();
            if(c == EOF) {
                fputs("error: inf proc: 演算子が壊れています\n", stderr);
                exit(1);
            }
            ope[i] = c;
        }
        if(fgetc() != ' ') {
            fputs("error: inf proc: 演算子が壊れています\n", stderr);
            exit(1);
        }
        ope[3] = '\0';
        if(strcmp(ope, "ttl") == 0) {
            int i = 0;
            while((c = fgetc()) == '\n') {
                if(c == EOF) {
                    fputs("error: inf proc: 被演算子が途中で終わっています\n", stderr);
                    exit(1);
                }
                pffr.info.title[i] = c;
                i ++;
            }
            pffr.info.title[i] = '\0';
        }
        if(strcmp(ope, "ttl") == 0) {
            int i = 0;
            while((c = fgetc()) == '\n') {
                if(c == EOF) {
                    fputs("error: inf proc: 被演算子が途中で終わっています\n", stderr);
                    exit(1);
                }
                pffr.info.title[i] = c;
                i ++;
            }
            pffr.info.title[i] = '\0';
        } else if(strcmp(ope, "ath") == 0) {
            int i = 0;
            while((c = fgetc()) == '\n') {
                if(c == EOF) {
                    fputs("error: inf proc: 被演算子が途中で終わっています\n", stderr);
                    exit(1);
                }
                pffr.info.author[i] = c;
                i ++;
            }
            pffr.info.author[i] = '\0';
        }
    }

 // ページプロセス
    if(acsPage == -1) {
        fputs("error: acs proc: ページプロセスが指定されていません\n", stderr);
        exit(1);
    } else if(fseek(pf, acsPage, SEEK_SET) != 0) {
        fputs("error: acs proc: 指定されたページプロセスは存在しません\n", stderr);
        exit(1);
    }
    for(int i = 0; i < 3; i ++) {
        c = fgetc();
        if(c == EOF) {
            fputs("error: page proc: 演算子が壊れています\n", stderr);
            exit(1);
        }
        ope[i] = c;
    }
    if(strcmp(ope, "tnp") != 0 || fgetc() != ' ') {
        fputs("error: page proc: 演算子が間違っています\n", stderr);
        exit(1);
    }
    char tnpStr[255];
    for(int i = 0; i < 254; i ++) {
        c = fgetc();
        if(c == EOF) {
            fputs("error: page proc: tnp被演算子が間違っています\n", stderr);
            exit(1);
        } else if(c == '\n') {
            tnpStr[i] = '\0';
            break;
        } else {
            tnpStr[i] = (char)c;
        }
    }
    pffr->pageSize = atoi(tnpStr);
    if(pffr->pageSize < 1) {
        fputs("error: page proc: tnp被演算子が数字ではありません\n", stderr);
        exit(1);
    }
    pffr->page = (ProcPage *)malloc(sizeof(ProcPage) * pffr->pageSize);

    fclose(pf);
    return;
}

void fgetToken(FILE *f, char *str, int strSize) {
    int c;
    int i;
    int eot = 0;

    c = fgetc(f);
    if(c == EOF) {
        goto error;
    } else if(isSpace(c)) {
        while(!isSpace(c)) {
            c = fgetc(f);
            if(c == EOF) {
                goto error;
            }
        }
    }
    str[0] = c;
    for(i = 1; i < strSize - 1; i ++) {
        c = fgetc();
        if(c == EOF) {
            goto error;
        } else if(isSpace(c)) {
            str[i] = '\0';
            eot = 1;
            break;
        }
        str[i] = c;
    }
    if(!eot) {
        goto error;
    }

error:
    fputs("error: no take token\n", stderr);
    exit(1);
}
