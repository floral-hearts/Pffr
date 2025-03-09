#ifndef PFFR_H
#define PFFR_H

#include "proc-info.h"
#include "proc-page.h"

#define isSpace(c) ((c) == ' ' || (c) == '\t' || (c) == '\v')
#define isNext(c) ((c) == '\n' || (c) == '\r')

typedef struct tagPffr {
    char path[255];
    int version;
    long acsInfo;
    long acsPage;
    ProcInfo info;
    ProcPage *page;
    int pageSize;
} Pffr;

void createDefaultFile(Pffr *pffr);
void getFileContent(Pffr *pffr);
void magic(Pffr *pffr, FILE *pf);
void access(Pffr *pffr, FILE *pf);
void information(Pffr *pffr, FILE *pf);
void page(Pffr *pffr, FILE *pf);
void kid(Pffr *pffr, FILE *pf);
void fgetToken(Pffr *pffr, FILE *f, char *str, int strSize);
void error(Pffr *pffr, char *msg);

#endif
