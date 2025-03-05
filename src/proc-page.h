#ifndef PROC__PAGE_H
#define PROC__PAGE_H

#include "color.h"
#include "pos.h"

typedef struct tagProcPage {
    Color background;
    Pos size;
} ProcPage;

ProcPage setDefaultProcPage(void);

#endif
