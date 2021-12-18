#ifndef SANTASDAYOFF_SANTA_H
#define SANTASDAYOFF_SANTA_H

#include "r/ro_types.h"
#include "pixelparticles.h"

typedef struct {
    PixelParticles *particles_ref;

    struct {
        vec2 gift_pos;
    } out;
    
    struct {
        RoSingle ro;
        float time;
    } L;
} Santa;


Santa *santa_new(PixelParticles *particles);

void santa_kill(Santa **self_ptr);

void santa_update(Santa *self, float dtime);

void santa_render(const Santa *self, const mat4 *cam_mat);

#endif //SANTASDAYOFF_SANTA_H
