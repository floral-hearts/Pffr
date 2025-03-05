#include "proc-info.h"

#include <string.h>

ProcInfo setDefaultProcInfo(void) {
    ProcInfo ret;
    strcpy(ret.title, "default title");
    strcpy(ret.author, "default author");
    return ret;
}
