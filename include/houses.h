#ifndef SANTASDAYOFF_HOUSES_H
#define SANTASDAYOFF_HOUSES_H

#include "r/ro_types.h"
#include "pixelparticles.h"

#define HOUSES_MAX 32

typedef struct {
    PixelParticles *particles_ref;
    
    struct {
        float santa_pos;
        bool gifts[8];
    } in;
    
    struct {
        int missed_gifts;
        int right_gifts;
    } out;
    
    struct {
        RoBatch ro;
        RoBatch gifts;
        RoText number[HOUSES_MAX];
        int last; // wait to reset
        int next; // next or current for santa
        float particles_time;
    } L;
} Houses;


Houses *houses_new(PixelParticles *particles);

void houses_kill(Houses **self_ptr);

void houses_update(Houses *self, float dtime);

void houses_render(const Houses *self, const mat4 *cam_mat);

#endif //SANTASDAYOFF_HOUSES_H
