#include "player.h"

bool selected = false;
vec2 offset;

DeccanColor ColorList_White = (DeccanColor){255, 255, 255, 255}, ColorList_Black = (DeccanColor){0, 0, 0, 255},
            ColorList_Red = (DeccanColor){255, 0, 0, 255}, ColorList_Green = (DeccanColor){0, 255, 0, 255},
            ColorList_Orange = (DeccanColor){255, 165, 0, 255};

void _player_begin(DeccanGameObject this) {
    DeccanObjectInfo *info = DE_ObjectGetComponent(this, "Info");

    DE_ObjectSetComponent(this, "Color", &(DeccanColor){255, 0, 0, 255});
    DE_ObjectSetComponent(
        this, "Collider", &(DeccanCompCollider){.type = COLLIDER_Rect, .rect = {[0] = 0, [1] = 0, [2] = 50, [3] = 50}});
    DE_ObjectSetComponent(this, "State2D", &(DeccanCompState2D){.position = {[0] = 200, [1] = 200}});

}

void _player_step(DeccanGameObject this) {
    static int32_t SpeedModifier = 5;

    DeccanCompState2D *state = DE_ObjectGetComponent(this, "State2D");
    DeccanColor *color = DE_ObjectGetComponent(this, "Color");

    if (DE_InputKeyHeld(DECCAN_KEY_W)) {
        state->position[1] -= SpeedModifier;
    }
    else if (DE_InputKeyHeld(DECCAN_KEY_S)) {
        state->position[1] += SpeedModifier;
    }
    else if (DE_InputKeyHeld(DECCAN_KEY_A)) {
        state->position[0] -= SpeedModifier;
    }
    else if (DE_InputKeyHeld(DECCAN_KEY_D)) {
        state->position[0] += SpeedModifier;
    }

    if (DE_InputKeyReleased(DECCAN_KEY_O)) {
        SpeedModifier -= 1;
    }
    else if (DE_InputKeyReleased(DECCAN_KEY_P)) {
        SpeedModifier += 1;
    }

    /* Center the camera on player */
    // Camera_CenterOn(this);

    vec2 pos;
    DE_InputGetMousePos(pos);
    if (DE_CompColliderCheckObjectWithVector(this, pos)) {
        *color = ColorList_Orange;
        if (DE_InputButtonDown(DECCAN_BUTTON_LEFT)) {
            selected = true;
            offset[0] = pos[0] - state->position[0];
            offset[1] = pos[1] - state->position[1];
        }
        else if (DE_InputButtonUp(DECCAN_BUTTON_LEFT)) {
            selected = false;
        }
    }

    if (selected) {
        state->position[0] = pos[0] - offset[0];
        state->position[1] = pos[1] - offset[1];
    }
}

void _player_render(DeccanGameObject this) {
    DeccanCompState2D *state = DE_ObjectGetComponent(this, "State2D");
    DeccanColor *color = DE_ObjectGetComponent(this, "Color");
}

void _player_end(DeccanGameObject this) {
    DE_UNUSED(this);
}

void _none_begin(DeccanGameObject this) {
    DeccanGameObject player = DE_SceneGetObject("main player");
    DeccanCompState2D *statePlayer = DE_ObjectGetComponent(player, "State2D");

    DE_ObjectSetComponent(this, "State2D",
        &(DeccanCompState2D){.position = {[0] = statePlayer->position[0], [1] = statePlayer->position[1]}});
    DE_ObjectSetComponent(
        this, "Collider", &(DeccanCompCollider){.type = COLLIDER_Rect, .rect = {[0] = 0, [1] = 0, [2] = 40, [3] = 40}});
    DE_ObjectSetTag(this, "isEnemy");
}

void _none_step(DeccanGameObject this) {
    DE_UNUSED(this);
}

void _none_render(DeccanGameObject this) {
    DeccanCompState2D *state = DE_ObjectGetComponent(this, "State2D");
}

void _none_end(DeccanGameObject this) {
    DE_UNUSED(this);
}
