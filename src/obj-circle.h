#ifndef OBJ__CIRCLE_H
#define OBJ__CIRCLE_H

#include "pos.h"

typedef struct tagObjCircle {
    Pos center;
    float radius;
} ObjCircle;

ObjCircle setDefaultObjCircle(void);

#endif
