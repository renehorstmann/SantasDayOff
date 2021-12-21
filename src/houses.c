#include "r/ro_batch.h"
#include "r/ro_text.h"
#include "r/texture.h"
#include "u/pose.h"

#include "rhc/alloc.h"
#include "mathc/utils/random.h"

#include "houses.h"


Houses *houses_new(PixelParticles *particles) {
    Houses *self = rhc_calloc(sizeof *self);
    
    self->particles_ref = particles;
    
    self->L.ro = ro_batch_new(HOUSES_MAX, r_texture_new_file(2, 1, "res/houses.png"));
    
    self->L.gifts = ro_batch_new(HOUSES_MAX*3, r_texture_new_file(4, 2, "res/gifts.png"));
    
    
    for(int i=0; i<HOUSES_MAX; i++) {
        self->L.number[i] = ro_text_new_font55(3);
        ro_text_set_color(&self->L.number[i], (vec4) {{0.5, 0.5, 0.5, 1}});
    }
    
    for(int i=0; i<HOUSES_MAX; i++) {
        self->L.ro.rects[i].pose = u_pose_new(NAN, NAN, 32, 32);
        
    }
    
    return self;
}

void houses_kill(Houses **self_ptr) {
    Houses *self = *self_ptr;
    if(!self)
        return;
    ro_batch_kill(&self->L.ro);
    ro_batch_kill(&self->L.gifts);
    for(int i=0; i<HOUSES_MAX; i++) {
        ro_text_kill(&self->L.number[i]);
    }
    rhc_free(self);
    *self_ptr = NULL;
}

void houses_update(Houses *self, float dtime) {
    
    
}

void houses_render(const Houses *self, const mat4 *cam_mat) {
    ro_batch_render(&self->L.ro, cam_mat, true);
    ro_batch_render(&self->L.gifts, cam_mat, true);
    for(int i=0; i<HOUSES_MAX; i++) {
        ro_text_render(&self->L.number[i], cam_mat);
    }
}

