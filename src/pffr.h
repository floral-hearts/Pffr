#ifndef PFFR_H
#define PFFR_H

#include "proc-info.h"
#include "proc-page.h"

typedef struct tagPffr {
    char path[255];
    int version;
    ProcInfo info;
    ProcPage *page;
    int pageSize;
} Pffr;

void createDefaultFile(Pffr *pffr);
void getFileContent(Pffr *pffr);

#endif
