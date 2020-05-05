/* Deccan Game Engine - C11 2D SDL Game Engine.
 * Copyright 2020 Ayush Bardhan Tripathy
 *
 * This software is licensed under MIT License.
 * See LICENSE.md included with this package for more info.
 */

#pragma once
#include "../config.h"
#include "../renderer/color.h"
#include "../physics/collider.h"
#include "../utils/message.h"
#include "../utils/vector.h"

#ifndef DECCAN_OBJ_MSG_LENGTH
    #define DECCAN_OBJ_MSG_LENGTH 50
#endif

#ifndef DECCAN_OBJ_MSG_COUNT
    #define DECCAN_OBJ_MSG_COUNT 100
#endif

#define obj_func(x) void (*x)(DE_GameObject *object)

typedef struct DE_GameObject DE_GameObject;
typedef struct DE_GameObject {
    struct { char *name, *type; } info;     /* Basic information about the object */ 
    DE_Vector2f position;               /* Positional info of the object */
    DE_Vector2f size;                   /* Size of the rect bounding the object */
    DE_Vector2f transform;              /* Transformation info */
    struct { bool dead, hidden; } status;   /* Status */
    DE_Collider collider;               /* Collider info */
    union {
        struct { DE_Color color; };     /* Color value for shape rendering */
    };

    DE_msgbuf msg;
    void (*SendMessage)(DE_GameObject *obj, const char *msg);
    bool (*ReceiveMessage)(DE_GameObject *obj, const char *msg);

    bool is_beginning;
    obj_func(AtFirstFrame);
    obj_func(AtBeginning);
    obj_func(AtStep);
    obj_func(AtRender);
    obj_func(AtEnd);
} DE_GameObject;

DE_GameObject *DE_Object_NewObject(const char *name, const char *type, obj_func(af), obj_func(ab), obj_func(as), obj_func(ar), obj_func(ae));
void DE_Object_InstantiateObject(DE_GameObject *object);
DE_GameObject *DE_Object_GetObject(const char *name);
void DE_Object_GetObjectOfType(const char *name, void(*func)(DE_GameObject *obj));

static inline void NULL_OBJFUNC(DE_GameObject *obj) { }

#undef obj_func