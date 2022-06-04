#include "r/ro_particle.h"
#include "r/texture.h"
#include "u/pose.h"
#include "mathc/float.h"
#include "mathc/sca/int.h"
#include "mathc/utils/random.h"
#include "rhc/alloc.h"
#include "snow.h"

#define NUM 4096
#define SIZE 2.0

#define RENEW_PS 248

static vec2 get_start(float left, float right, float top) {
    vec2 s;
    s.x = sca_random_range(left, right+2048);
    s.y = top;
    return s;
}


Snow *snow_new() {
    Snow *self = rhc_calloc(sizeof *self);
    
    self->L.ro = ro_particle_new(NUM, r_texture_new_white_pixel());

    for (int i = 0; i < self->L.ro.num; i++) {
        rParticleRect_s *p = self->L.ro.rects + i;
        
        p->pose = u_pose_new(NAN, NAN, SIZE, SIZE);
        
        float val = sca_random_range(0.7, 1.0);
        p->color = (vec4) {{val, val, val, sca_random_range(0.25, 0.75)}};
        
        p->speed.x = sca_random_noise(0, 32);
        p->speed.y = sca_random_noise(-32, 8);
        
        p->start_time = NAN;
    }
    
    return self;
}

void snow_kill(Snow **self_ptr) {
    Snow *self = *self_ptr;
    if (!self)
        return;
    ro_particle_kill(&self->L.ro);
    rhc_free(self);
    *self_ptr = NULL;
}

void snow_update(Snow *self, float dtime, float left, float right, float top) {
    self->L.time += dtime;
    self->L.particle_time += dtime * RENEW_PS;
    
    int add = (int) self->L.particle_time;
    self->L.particle_time -= add;
    
    for(int i=0; i<add; i++) {
        self->L.last++;
        if(self->L.last>=NUM) 
            self->L.last = 0;
            
        vec2 pos = get_start(left, right, top);
        u_pose_set_xy(&self->L.ro.rects[self->L.last].pose, pos.x, pos.y);
        self->L.ro.rects[self->L.last].start_time = self->L.time;
    }
}

void snow_render(const Snow *self, const mat4 *cam_mat) {
    ro_particle_render(&self->L.ro, self->L .time, cam_mat, true);
}
