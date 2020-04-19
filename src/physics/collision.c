/* Deccan Game Engine - C11 2D SDL Game Engine.
 * Copyright 2020 Ayush Bardhan Tripathy
 *
 * This software is licensed under MIT License.
 * See LICENSE.md included with this package for more info.
 */

#include "collision.h"

Deccan_Collider _priv_Collision_new_rect_collider(Deccan_PosRect rect) {
    Deccan_Collider coll;
    coll.type = Deccan_ColliderType_Rect;
    coll.rect = rect;
    return coll;
}

Deccan_Collider _priv_Collision_new_circle_collider(Deccan_Circle circle) {
    Deccan_Collider coll;
    coll.type = Deccan_ColliderType_Circle;
    coll.circle = circle;
    return coll;
}

bool _priv_Collision_test_vec_from(Deccan_GameObject *obj, Deccan_Vector2i vec) {
    switch(obj->collider.type) {
        case Deccan_ColliderType_Vec : {
            return obj->collider.vec.x == vec.x && obj->collider.vec.y == vec.y;
        }
        case Deccan_ColliderType_Rect: {
            int32_t x1 = obj->position.x + obj->collider.rect.x1;
            int32_t y1 = obj->position.y + obj->collider.rect.y1;
            int32_t x2 = x1 + obj->collider.rect.x2;
            int32_t y2 = y1 + obj->collider.rect.y2;

            return vec.x > x1 && vec.x < x2 && vec.y > y1 && vec.y < y2;
        }
        case Deccan_ColliderType_Circle: {
            int32_t x = obj->position.x + obj->collider.circle.x;
            int32_t y = obj->position.y + obj->collider.circle.y;
            int32_t r = obj->collider.circle.radius;
            
            int64_t distance = ((x-vec.x)*(x-vec.x)) + ((y-vec.y)*(y-vec.y));
            if(distance < r*r) { return true; }
        }
        default: { return false; }
    }
}

bool _priv_Collision_test_from(Deccan_GameObject *obj1, Deccan_GameObject *obj2) {
    /* Possible cases where v = vector, r = rect and c = circle
        v <-> v      r <-> r      c <-> c
        v <-> r      c <-> v      r <-> c 
    */
    
    int type1 = obj1->collider.type;
    int type2 = obj2->collider.type;
    
    /* Matching cases */
    if(type1 == type2) {
        switch(type1) {
            case Deccan_ColliderType_Vec: {
                return obj1->position.x + obj1->collider.vec.x == 
                       obj2->position.x + obj2->collider.vec.x &&
                       obj1->position.y + obj1->collider.vec.y == 
                       obj2->position.y + obj2->collider.vec.y;
            }
            case Deccan_ColliderType_Rect: { 
                int32_t x11 = obj1->position.x + obj1->collider.rect.x1;
                int32_t x12 = obj1->position.x + obj1->collider.rect.x2;
                int32_t y11 = obj1->position.y + obj1->collider.rect.y1;
                int32_t y12 = obj1->position.y + obj1->collider.rect.y2;

                int32_t x21 = obj2->position.x + obj2->collider.rect.x1;
                int32_t x22 = obj2->position.x + obj2->collider.rect.x2;
                int32_t y21 = obj2->position.y + obj2->collider.rect.y1;
                int32_t y22 = obj2->position.y + obj2->collider.rect.y2;

                return x11 < x22 && x12 > x21 && y11 < y22 && y12 > y21;
            }
            case Deccan_ColliderType_Circle: {
                int32_t x1 = obj1->position.x + obj1->collider.circle.x;
                int32_t y1 = obj1->position.y + obj1->collider.circle.y;
                int32_t r1 = obj1->collider.circle.radius;
                
                int32_t x2 = obj2->position.x + obj2->collider.circle.x;
                int32_t y2 = obj2->position.y + obj2->collider.circle.y;
                int32_t r2 = obj2->collider.circle.radius;

                int64_t distance = ((x1-x2)*(x1-x2)) + ((y1-y2)*(y1-y2));
                return distance < (r1+r2)*(r1+r2);
            }
        }
    }

    /* Remaining cases */
    if(type1 == Deccan_ColliderType_Vec && type2 == Deccan_ColliderType_Rect) { }
    else if(type1 == Deccan_ColliderType_Circle && type2 == Deccan_ColliderType_Vec) { }

    bool case_1 = (type1 == Deccan_ColliderType_Rect && type2 == Deccan_ColliderType_Circle);
    bool case_2 = (type1 == Deccan_ColliderType_Circle && type2 == Deccan_ColliderType_Rect);
    
    if(case_1 || case_2) {
        int64_t distance;
        int32_t cx, cy;
        int32_t x1, y1, w, h, x2, y2;
    
        if(case_1) {
            x1 = obj1->position.x + obj1->collider.rect.x1;
            y1 = obj1->position.y + obj1->collider.rect.y1;
            w  = obj1->collider.rect.x2;
            h  = obj1->collider.rect.y2;

            x2 = obj2->position.x + obj2->collider.circle.x;
            y2 = obj2->position.y + obj2->collider.circle.y;
        }
        else {
            x1 = obj2->position.x + obj2->collider.rect.x1;
            y1 = obj2->position.y + obj2->collider.rect.y1;
            w  = obj2->collider.rect.x2;
            h  = obj2->collider.rect.y2;

            x2 = obj1->position.x + obj1->collider.circle.x;
            y2 = obj1->position.y + obj1->collider.circle.y;
        }

        if(x2 < x1) { cx = x1; }
        else if(x2 > x1 + w) { cx = x1 + w; }
        else { cx = x2; }

        if(y2 < y1) { cy = y1; }
        else if(y2 > y1 + h) { cy = y1 + h; }
        else { cy = y2; }

        distance = ((cx-x2)*(cx-x2)) + ((cy-y2)*(cy-y2));
        return distance < (obj2->collider.circle.radius*obj2->collider.circle.radius);
    }

    return false;
}

bool _priv_Collision_test(const char *name1, const char *name2) {
    Deccan_GameObject *obj1 = Deccan_Object.get_object(name1);
    Deccan_GameObject *obj2 = Deccan_Object.get_object(name2);
    
    return Deccan_Collision.test_from(obj1, obj2);
}