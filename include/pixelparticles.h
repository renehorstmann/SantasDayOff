#ifndef SANTASDAYOFF_PIXELPARTICLES_H
#define SANTASDAYOFF_PIXELPARTICLES_H

#include "r/rect.h"
#include "u/color.h"
#include "r/ro_types.h"

typedef struct {
    float time;

    struct {
        RoParticle ro;
        int next;
    } L;
} PixelParticles;


PixelParticles *pixelparticles_new();

void pixelparticles_kill(PixelParticles **self_ptr);

void pixelparticles_update(PixelParticles *self, float dtime);

void pixelparticles_render(const PixelParticles *self, const mat4 *cam_mat);

void pixelparticles_add(PixelParticles *self, const rParticleRect_s *particles, int n);

#endif //SANTASDAYOFF_PIXELPARTICLES_H
