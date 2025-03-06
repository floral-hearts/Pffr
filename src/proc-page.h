#ifndef PROC__PAGE_H
#define PROC__PAGE_H

#include "color.h"
#include "obj.h"
#include "pos.h"

typedef struct tagProcPage {
    Color background;
    Pos size;
    Obj *obj;
    int objSize;
} ProcPage;

ProcPage setDefaultProcPage(void);

#endif
