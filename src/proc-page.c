#include "proc-page.h"

ProcPage setDefaultProcPage(void) {
    ProcPage ret;
    ret.color.red = 0x00;
    ret.color.green = 0x00;
    ret.color.blue = 0x00;
    ret.pos.x = 297f;
    ret.pos.y = 210f;
    return ret;
}
