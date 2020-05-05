/* Deccan Game Engine - C11 2D SDL Game Engine.
 * Copyright 2020 Ayush Bardhan Tripathy
 *
 * This software is licensed under MIT License.
 * See LICENSE.md included with this package for more info.
 */

#include "object.h"
#include "scene.h"

void _msg_send(DE_GameObject *obj, const char *msg) {
    DE_msg_send(&obj->msg, msg);
}

bool _msg_receive(DE_GameObject *obj, const char *msg) {
    return DE_msg_receive(&obj->msg, msg);
}

#define obj_func(x) void (*x)(DE_GameObject *object)

DE_GameObject *DE_Object_NewObject(
    const char *name, const char *type, 
    obj_func(af), obj_func(ab), obj_func(as), obj_func(ar), obj_func(ae)) {
    
    DE_GameObject *obj = DE_new(DE_GameObject, 1);
    
    obj->info.name = DE_newstring(name);
    obj->info.type = DE_newstring(type);
    
    DE_msg_init(&obj->msg, DECCAN_OBJ_MSG_COUNT, DECCAN_OBJ_MSG_LENGTH);
    obj->SendMessage = _msg_send;
    obj->ReceiveMessage = _msg_receive;
    
    obj->is_beginning = true;
    obj->AtFirstFrame = af;
    obj->AtBeginning = ab;
    obj->AtStep = as;
    obj->AtRender = ar;
    obj->AtEnd = ae;

    return obj;
}

#undef obj_func

void DE_Object_InstantiateObject(DE_GameObject *object) {
    DE_GameScene *scene = DE_Scene_CurrentScene(); 
    if(object == NULL) { return; }
    if(stbds_arrput(scene->objects, object) != object) {
        DE_report("Cannot instantiate object: %s", object->info.name); return;
    }
    scene->object_count++;
}

DE_GameObject *DE_Object_GetObject(const char *name) {
    DE_GameScene *scene = DE_Scene_CurrentScene();
    for(int i=0; i<scene->object_count; i++) {
        if(!strcmp(scene->objects[i]->info.name, name)) {
            return scene->objects[i];
        }
    }
    DE_report("GameObject not found: %s", name);
}

void DE_Object_GetObjectOfType(const char *name, void(*func)(DE_GameObject *obj)) {
    DE_GameScene *scene = DE_Scene_CurrentScene();
    for(int i=0; i<scene->object_count; i++) {
        if(!strcmp(scene->objects[i]->info.type, name)) {
            func(scene->objects[i]);
        }
    }
}