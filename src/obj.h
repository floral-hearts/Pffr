#ifndef OBJ_H
#define OBJ_H

#include "color.h"
#include "obj-circle.h"
#include "obj-rectangle.h"

typedef union tagObjContent {
    ObjCircle circle;
    ObjRectangle rectangle;
} ObjContent;

typedef enum tagObjType {
    OBJ_TYPE_CIRCLE,
    OBJ_TYPE_RECTANGLE,
} ObjType;

typedef struct tagObj {
    char name[255]
    Color color;
    ObjType type;
    ObjContent content;
} Obj;

#endif
