#ifndef PFFR_H
#define PFFR_H

#include "proc-info.h"

typedef struct tagPffr {
    char path[255];
    ProcInfo info;
} Pffr;

void createDefaultFile(Pffr pffr);

#endif
