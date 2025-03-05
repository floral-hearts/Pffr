#ifndef PFFR_H
#define PFFR_H

#include "proc-info.h"
#include "proc-page.h"

typedef struct tagPffr {
    char path[255];
    ProcInfo info;
    ProcPage *page;
} Pffr;

void createDefaultFile(Pffr pffr);

#endif
