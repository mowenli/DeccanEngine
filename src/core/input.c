/* Deccan Game Engine - C11 2D SDL Game Engine.
 * Copyright 2020 Ayush Bardhan Tripathy
 *
 * This software is licensed under MIT License.
 * See LICENSE.md included with this package for more info.
 */

#include "input.h"

DE_KeyState DE_Input_get_key(int key_code) {
    DE_KeyState key;
    DE_Info *engine = DE_Core_get_global_engine();

    if(engine->curr_keys[key_code]) { 
        if(!engine->prev_keys[key_code]) { key.is_pressed = true; }
        else { key.is_pressed = false; }
        key.is_held = true; 
        key.is_released = false;
    }
    else {
        if(engine->prev_keys[key_code]) { key.is_released = true; }
        else { key.is_released = false; }
        
        key.is_held = false;
        key.is_pressed = false;
    }

    return key;
}

DE_MouseState DE_Input_get_mouse_button(int button_code) {
    DE_MouseState button = {false, false};
    DE_Info *engine = DE_Core_get_global_engine();

    if(engine->event.button.button == button_code) {
        if(engine->event.type == SDL_MOUSEBUTTONDOWN) { button.is_down = true; }
        else if(engine->event.type == SDL_MOUSEBUTTONUP) { button.is_up = true; }
    }

    return button;
}

DE_Vector2i DE_Input_get_mouse_pos() {
    DE_Vector2i pos;
    SDL_GetMouseState(&pos.x, &pos.y);
    return pos;
}

DE_Vector2i DE_Input_get_relative_mouse_pos() {
    DE_Vector2i pos;
    DE_Vector2i cam = DE_Core_get_global_engine()->camera; 
    
    SDL_GetMouseState(&pos.x, &pos.y);
    pos.x += cam.x;
    pos.y += cam.y;
    return pos;
}

int DE_Input_mouse_scroll_horizontal() {
    DE_Info *engine = DE_Core_get_global_engine();
    if(engine->event.type == SDL_MOUSEWHEEL) {
        if(engine->event.wheel.direction == SDL_MOUSEWHEEL_NORMAL) { return engine->event.wheel.x; }
        else { return -engine->event.wheel.x; }
    }
    return 0;
}

int DE_Input_mouse_scroll_vertical() {
    DE_Info *engine = DE_Core_get_global_engine();
    if(engine->event.type == SDL_MOUSEWHEEL) {
        if(engine->event.wheel.direction == SDL_MOUSEWHEEL_NORMAL) { return engine->event.wheel.y; }
        else { return -engine->event.wheel.y ;}
    }
    else { return 0; }
}

bool DE_Input_key_pressed(int key_code) {
    DE_Info *engine = DE_Core_get_global_engine();
    return engine->curr_keys[key_code] && !engine->prev_keys[key_code];
}

bool DE_Input_key_released(int key_code) {
    DE_Info *engine = DE_Core_get_global_engine();
    return !engine->curr_keys[key_code] && engine->prev_keys[key_code];
}

bool DE_Input_key_held(int key_code) {
    return DE_Core_get_global_engine()->curr_keys[key_code];
}

bool DE_Input_button_down(int button_code) {
    DE_Info *engine = DE_Core_get_global_engine();
    return engine->event.type == SDL_MOUSEBUTTONDOWN &&
           engine->event.button.button == button_code;
}

bool DE_Input_button_up(int button_code) {
    DE_Info *engine = DE_Core_get_global_engine();
    return engine->event.type == SDL_MOUSEBUTTONUP &&
           engine->event.button.button == button_code;
}