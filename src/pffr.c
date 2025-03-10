#include "pffr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void writeContent(Pffr *pffr) {
    printf("path: %s\n", pffr->path);
    printf("version: %d\n", pffr->version);
    printf("acsInfo: %ld\n", pffr->acsInfo);
    printf("info:\n");
    printf(" * title: %s\n", pffr->info.title);
    printf(" * author: %s\n", pffr->info.author);
    printf("acsPage: %ld\n", pffr->acsPage);
    printf("page:\n");
    for(int i = 0; i < pffr->pageSize; i ++) {
        printf(" * [%d]", i);
        printf("    - acs: %ld\n", pffr->page[i].acs);
        printf("    - background: (%d, %d, %d)\n", pffr->page[i].background.red, pffr->page[i].background.green, pffr->page[i].background.blue);
        printf("    - size: (%d, %d)\n", pffr->page[i].size.x, pffr->page[i].size.y);
        printf("    - objSize: %d\n", pffr->page[i].objSize);
        printf("    - obj:\n")
        for(int j = 0; j < pffr->page[i].objSize; j ++) {
            printf("       ~ name: %s\n", pffr->page[i].obj[j].name);
            printf("       ~ color: (%d, %d, %d)\n", pffr->page[i].obj[j].color.red, pffr->page[i].obj[j].color.green, pffr->page[i].obj[j].color.blue);
            printf("       ~ type: %d\n", pffr->page[i].obj[j].type);
        }
    }
}

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
    pffr.pageSize = 0;
    magic(pffr, pf);
    access(pffr, pf);
    information(pffr, pf);
    page(pffr, pf);
    kid(pffr, pf);
    fclose(pf);
}

void magic(Pffr *pffr, FILE *pf) {
    char ope[4];
    char str255[255];

    ope[0] = '\0';
    fgetToken(pffr, pf, ope, 4);
    if(strcmp(ope, "doc") != 0) {
        error(pffr, "error: break operator\n");
    }
    fgetToken(pffr, pf, str255, 255);
    if(strcmp(ope, "Pffr") != 0) {
        error(pffr, "error: break 1st doc operand\n");
    }
    fgetToken(pffr, pf, str255, 255);
    if((pffr.version = atoi(str255)) != 1) {
        error(pffr, "error: cannnot open file\n");
    }
    fgetToken(pffr, pf, str255, 255);
    if(strcmp(str255, "\n\n\n") != 0) {
        error(pffr, "error: no find next process\n");
    }
}

void access(Pffr *pffr, FILE pf) {
    char ope[4];
    char key[4];
    char str255[255];

    ope[1] = 0;
    while(1) {
        fgetToken(pffr, pf, ope, 4);
        if(strcmp(ope, "acs") != 0) {
            error(pffr, "error: break operator\n");
        } else if(strcmp(ope, "\n\n\n") != 0) {
            break;
        }
        fgetToken(pffr, pf, key, 4);
        if(strcmp(key, "prc") != 0) {
            error(pffr, "error: break 1st acs operand\n");
        }
        fgetToken(pffr, pf, key, 4);
        fgetToken(pffr, pf, str255, 255);
        if(strcmp(key, "inf") == 0) {
            pffr.acsInfo = atol(str255);
        } else if(strcmp(key, "pag") == 0) {
            pffr.acsPage = atol(str255);
        } else {
            error(pffr, "error: break 2nd acs operand\n");
        }
    }
}

void information(Pffr *pffr, FILE *pf) {
    char ope[4];
    char str255[255];

    pffr.info = setDefaultProcInfo();
    while(pffr.acsInfo != -1) {
        if(fseek(pf, pffr.acsInfo, SEEK_SET) != 0) {
            error(pffr, "error: no find information process\n");
        }
        fgetToken(pffr, pf, ope, 4);
        if(strcmp(ope, "ttl") == 0) {
            fgetToken(pffr, pf, str255, 255);
            strcpy(pffr.info.title, str255);
        } else if(strcmp(ope, "ath") == 0) {
            fgetToken(pffr, pf, str255, 255);
            strcpy(pffr.info.author, str255);
        } else if(strcmp(ope, "\n\n\n") == 0) {
            break;
        } else {
            error(pffr, "error: break operator\n");
        }
    }
}

void page(Pffr *pffr, FILE *pf) {
    char ope[4];
    char key[4];
    char str255[255];
    int num;

    if(acsPage == -1 || fseek(pf, acsPage, SEEK_SET) != 0) {
        error(pffr, "error: no find page process\n");
    }
    fgetToken(pffr, pf, ope, 4);
    if(strcmp(ope, "tpg") != 0) {
        error(pffr, "error: break operator\n");
    }
    fgetToken(pffr, pf, str255, 255);
    pffr->pageSize = atoi(str255);
    if(pffr->pageSize < 1) {
        error(pffr, "error: break 1st tnp operand\n");
    }
    pffr->page = (ProcPage *)malloc(sizeof(ProcPage) * pffr->pageSize);
    for(int i = 0; i < pffr->pageSize; i ++) {
        pffr->page[i].objSize = 0;
    }

    while(1) {
        fgetToken(pffr, pf, ope, 4);
        if(strcmp(ope, "acs") != 0) {
            error(pffr, "error: break operator\n");
        }
        fgetToken(pffr, pf, key, 4);
        if(strcmp(key, "pag") != 0) {
            error(pffr, "error: break 1st acs operand\n");
        }
        fgetToken(pffr, pf, str255, 255);
        num = atoi(str255);
        if(!(0 <= num && num < pffr->pageSize)) {
            error(pffr, "error: break 2st acs operand\n");
        }
        fgetToken(pffr, pf, str255, 255);
        pffr->page[num].acs = atol(str255);
    }
}

void kid(Pffr *pffr, FILE *pf) {
    char ope[4];
    char key[4];
    char str255[255];

    int nextPage;
    int setedClr;
    int setedSiz;
    int setedTob;

    for(int i = 0; i < pffr.pageSize; i ++) {
        nextPage = setedClr = setedSiz = setedTob = 0;
        fseek(pf, pffr->page[i].acs, SEEK_SET);
        pffr->page[i] = setDefaultProcPage();
        while(1) {
            fgetToken(pffr, pf, ope, 4);
            if(strcmp(ope, "clr") == 0) {
                if(setedClr) {
                    error(pffr, "error: clr operator is duplicated\n");
                } else {
                    setedClr = 1;
                }
                fgetToken(pffr, pf, str255, 255);
                pffr->page[i].background.red = atoi(str255);
                fgetToken(pffr, pf, str255, 255);
                pffr->page[i].background.green = atoi(str255);
                fgetToken(pffr, pf, str255, 255);
                pffr->page[i].background.blue = atoi(str255);
            } else if(strcmp(ope, "siz") == 0) {
                if(setedSiz) {
                    error(pffr, "error: clr operator is duplicated\n");
                } else {
                    setedSiz = 1;
                }
                fgetToken(pffr, pf, str255, 255);
                pffr->page[i].size.x = atoi(str255);
                fgetToken(pffr, pf, str255, 255);
                pffr->page[i].size.y = atoi(str255);
            } else if(strcmp(ope, "tob") == 0) {
                if(setedTob) {
                    error(pffr, "error: tob operator is duplicated\n");
                } else {
                    setedTob = 1;
                }
                fgetToken(pffr, pf, str255, 255);
                pffr->page[i].objSize = atoi(str255);
            } else {
                error(pffr, "error: break operator\n");
            }
            fgetToken(pffr, pf, key, 4);
            if(strcmp(key, "\n") == 0) {
            } else if(strcmp(key, "\n\n") == 0) {
                break;
            } else if(strcmp(key, "\n\n") == 0) {
                if(pffr->page[i].objSize == 0) {
                    nextPage = 1;
                } else if(pffr->page[i].objSize == 0) {
                    error(pffr, "error: no find object\n");
                }
            }
            if(pffr->page[i].objSize == 0) {
                error(pffr, "error: no find object\n");
            }
        }
        if(nextPage) {
            continue;
        }
        for(int j = 0; j < pffr->page[i].objSize; j ++) {
            fgetToken(pffr, pf, ope, 4);
            if(strcmp(ope, "obj") != 0) {
                error(pffr, "error: no find object\n");
            }
            fgetToken(pffr, pf, key, 4);
            if(strcmp(key, "cir") == 0) {
                pffr->page[i].obj[j].type = OBJ_TYPE_CIRCLE;
            } else if(strcmp(key, "rec") == 0) {
                pffr->page[i].obj[j].type = OBJ_TYPE_RECTANGLE;
            } else {
                error(pffr, "error: break 2nd obj operand\n");
            }
            fgetToken(pffr, pf, str255, 255);
            strcpy(pffr->page[i].obj[j].name, str255);
            fgetToken(pffr, pf, key, 4);
            if(strcmp(key, "\n\n") != 0) {
                error(pffr, "error: no find obj content\n");
            }
            if(pffr->page[i].obj[j].type == OBJ_TYPE_CIRCLE) {
                int setedCenter = 0;
                int setedRadius = 0;
                while(1) {
                    fgetToken(pffr, pf, ope, 4);
                    if(strcmp(ope, "cnt") == 0) {
                        if(setedCenter) {
                            error(pffr, "error: cnt operator is duplicated\n");
                        } else {
                            setCenter = 1;
                        }
                        fgetToken(pffr, pf, str255, 255);
                        pffr->page[i].obj[j].content.circle.center.x = strtof(str255);
                        fgetToken(pffr, pf, str255, 255);
                        pffr->page[i].obj[j].content.circle.center.y = strtof(str255);
                    } else if(strcmp(ope, "rad") == 0) {
                        if(setedRadius) {
                            error(pffr, "error: rad operator is duplicated\n");
                        } else {
                            setedRadius = 1;
                        }
                        fgetToken(pffr, pf, str255, 255);
                        pffr->page[i].obj[j].content.circle.radius = strtof(str255);
                    } else {
                        error(pffr, "error: break operator\n");
                    }
                    fgetToken(pffr, pf, key, 4);
                    if(strcmp(key, "\n") == 0) {
                    } else if(strcmp(key, "\n\n") == 0 && j != pffr->page[i].objSize - 1) {
                        break;
                    } else if(strcmp(key, "\n\n\n") == 0 && j == pffr->page[i].objSize - 1) {
                        break;
                    } else {
                        error(pffr, "error: no find next obj\n");
                    }
                }
            } else if(pffr->page[i].obj[j].type == OBJ_TYPE_RECTANGLE) {
                int setedStart = 0;
                int setedEnd = 0;
                while(1) {
                    fgetToken(pffr, pf, ope, 4);
                    if(strcmp(ope, "sta") == 0) {
                        if(setedCenter) {
                            error(pffr, "error: sta operator is duplicated\n");
                        } else {
                            setCenter = 1;
                        }
                        fgetToken(pffr, pf, str255, 255);
                        pffr->page[i].obj[j].content.rectangle.start.x = strtof(str255);
                        fgetToken(pffr, pf, str255, 255);
                        pffr->page[i].obj[j].content.rectangle.start.y = strtof(str255);
                    } else if(strcmp(ope, "end") == 0) {
                        if(setedEnd) {
                            error(pffr, "error: end operator is duplicated\n");
                        } else {
                            setedEnd = 1;
                        }
                        fgetToken(pffr, pf, str255, 255);
                        pffr->page[i].obj[j].content.rectangle.end.x = strtof(str255);
                        fgetToken(pffr, pf, str255, 255);
                        pffr->page[i].obj[j].content.rectangle.end.y = strtof(str255);
                    } else {
                        error(pffr, "error: break operator\n");
                    }
                    fgetToken(pffr, pf, key, 4);
                    if(strcmp(key, "\n") == 0) {
                    } else if(strcmp(key, "\n\n") == 0 && j != pffr->page[i].objSize - 1) {
                        break;
                    } else if(strcmp(key, "\n\n\n") == 0 && j == pffr->page[i].objSize - 1) {
                        break;
                    } else {
                        error(pffr, "error: no find next obj\n");
                    }
                }
            }
        }
    }
}

void fgetToken(Pffr *pffr, FILE *f, char *str, int strSize) {
    int c;
    int i;
    int eot = 0;

    c = fgetc(f);
    if(c == EOF) {
        error(pffr, "error: no get token\n");
    } else if(isSpace(c)) {
        while(!isSpace(c)) {
            c = fgetc(f);
            if(c == EOF) {
                error(pffr, "error: no get token\n");
            }
        }
    }
    str[0] = c;
    for(i = 1; i < strSize - 1; i ++) {
        c = fgetc();
        if(c == EOF) {
            error(pffr, "error: no get token\n");
        } else if(isSpace(c)) {
            str[i] = '\0';
            eot = 1;
            break;
        }
        str[i] = c;
    }
    if(!eot) {
        error(pffr, "error: no get token\n");
    }
}

void error(Pffr *pffr, char *msg) {
    fputs(msg, stderr);
    for(int i = 0; i < pffr->pageSize; i ++) {
        for(int j = 0; j < pffr->page[i].objSize; j ++) {
            free(pffr->page[i].obj);
        }
        free(pffr->page);
    }
    exit(1);
}
