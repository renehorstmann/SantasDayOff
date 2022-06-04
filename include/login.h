#ifndef SANTASDAYOFF_LOGIN_H
#define SANTASDAYOFF_LOGIN_H

#include "e/input.h"
#include "r/ro_types.h"
#include "textinput.h"

#define LOGIN_NAME_MIN_LENGTH 3
#define LOGIN_NAME_MAX_LENGTH 16

typedef struct {
    eInput *input_ref;
    
    struct {
        char name[LOGIN_NAME_MAX_LENGTH + 1]; // + null terminator
        bool done;
    } out;
    struct {
        TextInput *textinput;

        RoSingle letter;
        RoText text;
        RoText name;
        mat4 name_click_box;
        RoSingle play;
    } L;
} Login;

Login *login_new(eInput *input);

void login_kill(Login **self_ptr);

void login_update(Login *self, ivec2 window_size, float dtime);

void login_render(const Login *self, const mat4 *hud_cam_mat);

void login_pointer_event(Login *self, ePointer_s pointer);

#endif //SANTASDAYOFF_LOGIN_H
