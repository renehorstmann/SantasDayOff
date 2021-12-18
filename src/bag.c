#include "r/ro_single.h"
#include "r/ro_batch.h"
#include "r/texture.h"
#include "u/pose.h"

#include "rhc/alloc.h"
#include "mathc/float.h"

#include "bag.h"

#define SPEED 100
#define ANIMATION_FPS 8

const static vec4 color_pressed = {0.5, 0.5, 0.5, 1.0};

Bag *bag_new() {
    Bag *self = rhc_calloc(sizeof *self);
    
    
    self->L.bg = ro_single_new(r_texture_new_file(4, 1, "res/bag.png"));
    
    self->L.gifts = ro_batch_new(8, r_texture_new_file(4, 2, "res/gifts.png"));
    
    return self;
}

void bag_kill(Bag **self_ptr) {
    Bag *self = *self_ptr;
    if(!self)
        return;
    ro_single_kill(&self->L.bg);
    ro_batch_kill(&self->L.gifts);
    rhc_free(self);
    *self_ptr = NULL;
}

void bag_update(Bag *self, float dtime, const Camera_s *cam) {
    memcpy(self->out.pressed, self->L.pressed, sizeof self->out.pressed);
    memset(self->L.pressed, 0, sizeof self->L.pressed);
    
    // todo, set poses
    
    
}

void bag_render(const Bag *self, const mat4 *cam_mat) {
    ro_single_render(&self->L.bg, cam_mat);
    ro_batch_render(&self->L.gifts, cam_mat, true);
}

void bag_pointer_event(Bag *self, ePointer_s pointer) {
    for(int i=0; i<8; i++) {
        if(pointer.action == E_POINTER_DOWN 
                && self->L.gifts.rects[i].color.r > 0.9
                && u_pose_aa_contains(self->L.gifts.rects[i].pose, pointer.pos.xy)) {
            self->L.gifts.rects[i].color = color_pressed;
            self->L.pressed[i] = true;
        }
        
        if(pointer.action == E_POINTER_UP
                || !u_pose_aa_contains(self->L.gifts.rects[i].pose, pointer.pos.xy)) {
            self->L.gifts.rects[i].color = vec4_set(1);
        }
    }
}
