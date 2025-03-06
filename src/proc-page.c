#include "proc-page.h"

#include <stdlib.h>

ProcPage setDefaultProcPage(void) {
    ProcPage ret;
    ret.background.red = 0x00;
    ret.background.green = 0x00;
    ret.background.blue = 0x00;
    ret.size.x = 297.f;
    ret.size.y = 210.f;
    ret.obj = NULL;
    ret.objSize = 0;
    return ret;
}
