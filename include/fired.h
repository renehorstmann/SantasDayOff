#ifndef SANTASDAYOFF_FIRED_H
#define SANTASDAYOFF_FIRED_H

#include "e/input.h"
#include "r/ro_types.h"
#include "camera.h"
#include "showscore.h"


typedef struct {
    const char *name_ref;
    
    ShowScore *showscore;
    
    struct {
        float meter;
        int score;
    } in;
    
    struct {
        bool reset_game;
    } out;
    
    struct {
        RoSingle ro;
        RoSingle btn;
        RoText info;
        RoText credits;
    } L;
} Fired;

Fired *fired_new(const char *name_ref);

void fired_kill(Fired **self_ptr);

void fired_update(Fired *self, float dtime, const Camera_s *cam);

void fired_render(const Fired *self, const mat4 *cam_mat);

void fired_pointer_event(Fired *self, ePointer_s pointer);

#endif //SANTASDAYOFF_FIRED_H
