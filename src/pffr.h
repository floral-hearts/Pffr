#ifndef PFFR_H
#define PFFR_H

#include "proc-info.h"
#include "proc-page.h"

#define isSpace(c) ((c) == ' ' || (c) == '\t' || (c) == '\v')
#define isNext(c) ((c) == '\n' || (c) == '\r')

typedef struct tagPffr {
    char path[255];
    int version;
    ProcInfo info;
    ProcPage *page;
    int pageSize;
} Pffr;

void createDefaultFile(Pffr *pffr);
void getFileContent(Pffr *pffr);
void fgetToken(FILE *f, char *str, int strSize);

#endif
