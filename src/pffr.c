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
    pffr.acsInfo = -1;
    pffr.acsPage = -1;
    magic(pffr, pf);
    access(pffr, pf);
    information(pffr, pf);
    page(pffr, pf);
    fclose(pf);
}

void magic(Pffr *pffr, FILE *pf) {
    char ope[4];
    char str255[255];

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
        fputs("error: cannnot open file\n", stderr);
        exit(1);
    }
    fgetToken(pf, str255, 255);
    if(strcmp(str255, "\n\n\n") != 0) {
        fputs("error: no find next process\n", stderr);
        exit(1);
    }
}

void access(Pffr *pffr, FILE pf) {
    char ope[4];
    char key[4];
    char str255[255];

    ope[1] = 0;
    while(1) {
        fgetToken(pf, ope, 4);
        if(strcmp(ope, "acs") != 0) {
            fputs("error: break operator\n", stderr);
            exit(1);
        } else if(strcmp(ope, "\n\n\n") != 0) {
            break;
        }
        fgetToken(pf, key, 4);
        if(strcmp(key, "prc") != 0) {
            fputs("error: break 1st acs operand\n", stderr);
        }
        fgetToken(pf, key, 4);
        fgetToken(pf, str255, 255);
        if(strcmp(key, "inf") == 0) {
            pffr.acsInfo = atol(str255);
        } else if(strcmp(key, "pag") == 0) {
            pffr.acsPage = atol(str255);
        } else {
            fputs("error: break 2nd acs operand\n", stderr);
        }
    }
}

void information(Pffr *pffr, FILE *pf) {
    char ope[4];
    char str255[255];

    pffr.info = setDefaultProcInfo();
    while(pffr.acsInfo != -1) {
        if(fseek(pf, pffr.acsInfo, SEEK_SET) != 0) {
            fputs("error: no find information process\n", stderr);
            exit(1);
        }
        fgetToken(pf, ope, 4);
        if(strcmp(ope, "ttl") == 0) {
            fgetToken(pf, str255, 255);
            strcpy(pffr.info.title, str255);
        } else if(strcmp(ope, "ath") == 0) {
            fgetToken(pf, str255, 255);
            strcpy(pffr.info.author, str255);
        } else if(strcmp(ope, "\n\n\n") == 0) {
            break;
        } else {
            fputs("error: break operator\n", stderr);
        }
    }
}

void page(Pffr *pffr, FILE *pf) {
    char ope[4];
    char key[4];
    char str255[255];
    int num;

    if(acsPage == -1 || fseek(pf, acsPage, SEEK_SET) != 0) {
        fputs("error: no find page process\n", stderr);
        exit(1);
    }
    fgetToken(pf, ope, 4);
    if(strcmp(ope, "tpg") != 0) {
        fputs("error: break operator\n", stderr);
        exit(1);
    }
    fgetToken(pf, str255, 255);
    pffr->pageSize = atoi(str255);
    if(pffr->pageSize < 1) {
        fputs("error: break 1st tnp operand\n", stderr);
        exit(1);
    }
    pffr->page = (ProcPage *)malloc(sizeof(ProcPage) * pffr->pageSize);
    for(int i = 0; i < pffr->pageSize; i ++) {
        pffr->page[i].objSize = 0;
    }

    while(1) {
        fgetToken(pf, ope, 4);
        if(strcmp(ope, "acs") != 0) {
            fputs("error: break operator\n", stderr);
            free(pffr->page);
            exit(1);
        }
        fgetToken(pf, key, 4);
        if(strcmp(key, "pag") != 0) {
            fputs("error: break 1st acs operand\n", stderr);
            free(pffr->page);
            exit(1);
        }
        fgetToken(pf, str255, 255);
        num = atoi(str255);
        if(!(0 <= num && num < pffr->pageSize)) {
            fputs("error: break 2st acs operand\n", stderr);
            free(pffr->page);
            exit(1);
        }
        fgetToken(pf, str255, 255);
        pffr->page[num].acs = atol(str255);
    }
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
