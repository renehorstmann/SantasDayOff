#ifndef SANTASDAYOFF_SNOW_H
#define SANTASDAYOFF_SNOW_H

#include "r/ro_types.h"

typedef struct {
    struct {
        RoParticle ro;
        float time;
        float particle_time;
        int last;
    } L;
} Snow;


Snow *snow_new();

void snow_kill(Snow **self_ptr);

void snow_update(Snow *self, float dtime, float left, float right, float top);

void snow_render(const Snow *self, const mat4 *cam_mat);

#endif //SANTASDAYOFF_SNOW_H
