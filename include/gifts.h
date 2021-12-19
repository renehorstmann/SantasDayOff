#ifndef SANTASDAYOFF_GIFTS_H
#define SANTASDAYOFF_GIFTS_H

#include "r/ro_types.h"
#include "pixelparticles.h"

#define GIFTS_MAX 512

typedef struct {
    PixelParticles *particles_ref;
    
    struct {
        float start_speed;
    } in;
    
    struct {
        RoBatch ro;
        float speed[GIFTS_MAX];
        int last;
        float particles_time;
    } L;
} Gifts;


Gifts *gifts_new(PixelParticles *particles);

void gifts_kill(Gifts **self_ptr);

void gifts_update(Gifts *self, float dtime);

void gifts_render(const Gifts *self, const mat4 *cam_mat);

void gifts_add(Gifts *self, int id, vec2 start);

#endif //SANTASDAYOFF_GIFTS_H
