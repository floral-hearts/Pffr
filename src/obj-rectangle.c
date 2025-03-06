#include "obj-rectangle.h"

ObjRectangle setDefaultObjRectangle(void) {
    ObjRectangle ret;
    ret.start.x = 0.f;
    ret.start.y = 0.f;
    ret.end.x = 0.f;
    ret.end.y = 0.f;
    return ret;
}
