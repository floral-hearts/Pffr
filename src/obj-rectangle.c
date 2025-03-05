#include "obj-rectangle.h"

ObjRectangle setDefaultObjRectangle(void) {
    ObjRectangle ret;
    ret.start.x = 0f;
    ret.start.y = 0f;
    ret.end.x = 0f;
    ret.end.y = 0f;
    return ret;
}
