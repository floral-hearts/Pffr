#include "obj-circle.h"

ObjCircle setDefaultObjCircle(void) {
    ObjCircle ret;
    ret.center.x = 0f;
    ret.center.y = 0f;
    ret.radius = 0f;
    return ret;
}
