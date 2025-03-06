#include "obj-circle.h"

ObjCircle setDefaultObjCircle(void) {
    ObjCircle ret;
    ret.center.x = 0.f;
    ret.center.y = 0.f;
    ret.radius = 0.f;
    return ret;
}
