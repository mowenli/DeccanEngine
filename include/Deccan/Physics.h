/* Deccan Game Engine - C11 2D SDL Game Engine.
 * Copyright 2020 Ayush Bardhan Tripathy
 *
 * This software is licensed under MIT License.
 * See LICENSE.md included with this package for more info.
 */

#pragma once
#include <Deccan/Config.h>

typedef struct GameObject GameObject;

typedef enum {
    ColliderVec,
    ColliderRect,
    ColliderCircle
} ColliderType;

typedef struct Collider {
    ColliderType type;
    union {
        struct { Vector2i vec;  };
        struct { PosRect rect;  };
        struct { Circle circle; };
    };
} Collider;

Collider Collision_NewRectCollider(PosRect rect);
Collider Collision_NewCircleCollider(Circle circle);

bool Collision_VecVec(Vector2f *v1, Vector2f *v2);
bool Collision_RectRect(PosRect *r1, PosRect *r2);
bool Collision_CircleCircle(Circle *c1, Circle *c2);
bool Collision_VecRect(Vector2f *vec, PosRect *rect);
bool Collision_CircleVec(Circle *circle, Vector2f *vec);
bool Collision_RectCircle(PosRect *rect, Circle *circle);
bool Collision_ObjectObject(GameObject *obj1, GameObject *obj2);
bool Collision_ObjectVec(GameObject *obj, Vector2f *vec);
bool Collision_Test(const char *name1, const char *name2);