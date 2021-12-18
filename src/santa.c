#include "r/ro_single.h"
#include "r/texture.h"
#include "u/pose.h"

#include "rhc/alloc.h"

#include "santa.h"

#define SPEED 100
#define ANIMATION_FPS 8

Santa *santa_new(PixelParticles *particles) {
    Santa *self = rhc_calloc(sizeof *self);
    
    self->particles_ref = particles;
    
    self->L.ro = ro_single_new(r_texture_new_file(4, 1, "res/santa.png"));
    
    return self;
}

void santa_kill(Santa **self_ptr) {
    Santa *self = *self_ptr;
    if(!self)
        return;
    ro_single_kill(&self->L.ro);
    rhc_free(self);
    *self_ptr = NULL;
}

void santa_update(Santa *self, float dtime) {
    self->L.time += dtime;
    
    int frame = self->L.time * ANIMATION_FPS;
    frame %= 4;
    
    self->L.ro.rect.sprite.x = frame;
    
    float x = self->L.time * SPEED;
    x += 100;
    
    self->L.ro.rect.pose = u_pose_new(x, 300,
            -128, 64);
}

void santa_render(const Santa *self, const mat4 *cam_mat) {
    ro_single_render(&self->L.ro, cam_mat);
}
