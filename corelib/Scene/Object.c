/* Deccan Game Engine - C11 2D SDL Game Engine.
 * Copyright 2020, 2021 Ayush Bardhan Tripathy
 *
 * This software is licensed under MIT License.
 * See LICENSE.md included with this package for more info.
 */

#include "Object.h"
#include "Scene.h"
#include "Flecs.h"

/////////////////////////////////////////////////
// Initialization and instantiator functions
////////////////////////////////////////////////

void ObjectNoneFunc(DeccanGameObject obj) {
    DE_UNUSED(obj);
}

DeccanGameObject DE_ObjectNewObject(const char *name) {
    DeccanGameScene *scene = DE_SceneCurrentScene();
    DeccanGameObject obj;

    obj.entity = ecs_new_w_entity(scene->world, 0);

    DeccanObjectInfo info;
    info.visible = true;
    info.active = true;
    info.to_remove = false;
    info.is_beginning = true;
    info.AtFirstFrame = ObjectNoneFunc;
    info.AtBeginning = ObjectNoneFunc;
    info.AtStep = ObjectNoneFunc;
    info.AtRender = ObjectNoneFunc;
    info.AtEnd = ObjectNoneFunc;

    DE_ObjectSetName(obj, name);
    DE_ObjectSetComponent(obj, "Info", &info);
    DE_ObjectMakePrefab(obj);

    return obj;
}

void DE_ObjectDeleteObject(DeccanGameObject obj) {
    DeccanObjectInfo *info = DE_ObjectGetComponent(obj, "Info");
    info->to_remove = true;
}

void DE_ObjectFreeObject(DeccanGameObject obj) {
    DeccanGameScene *scene = DE_SceneCurrentScene();
    DeccanObjectInfo *info = DE_ObjectGetComponent(obj, "Info");

    if (info->active)
        info->AtEnd(obj);

    /* Free the messaging system */
    DE_VarQuit(&info->vars);

    /* Free */
    ecs_delete(scene->world, obj.entity);
}

bool DE_ObjectIsValid(DeccanGameObject obj) {
    return DE_ObjectHasTag(obj, "Info");
}

/////////////////////////////////////////////////
// Prefab and hierarchy functions
////////////////////////////////////////////////

DeccanGameObject DE_ObjectMakeCopy(DeccanGameObject object) {
    DeccanGameScene *scene = DE_SceneCurrentScene();

    DeccanGameObject object_inst;
    object_inst.entity = ecs_new_w_entity(scene->world, ECS_INSTANCEOF | object.entity);

    DeccanObjectInfo *info = DE_ObjectGetComponent(object_inst, "Info");
    info->is_beginning = true;
    info->active = true;
    DE_ObjectSetComponent(object_inst, "Info", info);

    return object_inst;
}

void DE_ObjectMakePrefab(DeccanGameObject object) {
    DeccanGameScene *scene = DE_SceneCurrentScene();
    ecs_add_entity(scene->world, object.entity, EcsPrefab);
}

void DE_ObjectSetParent(DeccanGameObject object, DeccanGameObject parent) {
    DeccanGameScene *scene = DE_SceneCurrentScene();
    ecs_add_entity(scene->world, object.entity, ECS_CHILDOF | parent.entity);
}

void DE_ObjectAddChild(DeccanGameObject object, DeccanGameObject child) {
    DE_ObjectSetParent(child, object);
}

/////////////////////////////////////////////////
// Update
////////////////////////////////////////////////

void DE_ObjectUpdate(DeccanGameObject obj) {
    DeccanObjectInfo *info = DE_ObjectGetComponent(obj, "Info");

    if (info->to_remove) {
        DE_ObjectFreeObject(obj);
        return;
    }

    if (!info->active)
        return;

    if (info->is_beginning) {
        /* Initialize messaging system */
        DE_VarInit(&info->vars);

        info->AtBeginning(obj);
        info->is_beginning = false;
        DE_ObjectSetComponent(obj, "Info", info);
    }
    else {
        info->AtStep(obj);
    }
}

void DE_ObjectRender(DeccanGameObject obj) {
    DeccanObjectInfo *info = DE_ObjectGetComponent(obj, "Info");

    if (!info->visible || !info->active)
        return;

    if (!info->is_beginning) {
        info->AtRender(obj);
    }
}

void DE_ObjectEnd(DeccanGameObject obj) {
    DeccanObjectInfo *info = DE_ObjectGetComponent(obj, "Info");

    if (!info->active)
        return;

    info->AtEnd(obj);
}

/////////////////////////////////////////////////
// Component
////////////////////////////////////////////////

void DE_ObjectSetComponent(DeccanGameObject obj, const char *name, void *component) {
    DE_FlecsSetComponent(obj.entity, name, component);
}

void *DE_ObjectGetComponent(DeccanGameObject obj, const char *name) {
    return DE_FlecsGetComponent(obj.entity, name);
}

void DE_ObjectRemoveComponent(DeccanGameObject obj, const char *name) {
    return DE_FlecsRemoveComponent(obj.entity, name);
}

void DE_ObjectSetName(DeccanGameObject obj, const char *name) {
    DeccanGameScene *scene = DE_SceneCurrentScene();
    ecs_set_ptr_w_entity(scene->world, obj.entity, FLECS__EEcsName, sizeof(EcsName), &(EcsName){DE_StringNew(name)});
}

const char *DE_ObjectGetName(DeccanGameObject obj) {
    DeccanGameScene *scene = DE_SceneCurrentScene();
    EcsName *name = ecs_get_mut_w_entity(scene->world, obj.entity, FLECS__EEcsName, NULL);
    return name->value;
}

void DE_ObjectSetTag(DeccanGameObject obj, const char *name) {
    DE_FlecsSetTag(obj.entity, name);
}

bool DE_ObjectHasTag(DeccanGameObject obj, const char *name) {
    return DE_FlecsHasTag(obj.entity, name);
}

/////////////////////////////////////////////////
// Setters and Getters
////////////////////////////////////////////////

bool DE_ObjectIsHidden(DeccanGameObject obj) {
    DeccanObjectInfo *info = DE_ObjectGetComponent(obj, "Info");
    return info->visible;
}

void DE_ObjectHide(DeccanGameObject obj, bool hide) {
    DeccanObjectInfo *info = DE_ObjectGetComponent(obj, "Info");
    info->visible = hide;
    DE_ObjectSetComponent(obj, "Info", info);
}

bool DE_ObjectIsActive(DeccanGameObject obj) {
    DeccanObjectInfo *info = DE_ObjectGetComponent(obj, "Info");
    return info->active;
}

void DE_ObjectActivate(DeccanGameObject obj, bool act) {
    DeccanObjectInfo *info = DE_ObjectGetComponent(obj, "Info");
    info->active = act;
    DE_ObjectSetComponent(obj, "Info", info);
}
