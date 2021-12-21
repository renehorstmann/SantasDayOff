#include "r/ro_batch.h"
#include "r/ro_text.h"
#include "r/texture.h"
#include "u/pose.h"

#include "rhc/alloc.h"
#include "mathc/utils/random.h"

#include "houses.h"


#define KILL_LEFT -512  // <= in.santa_oos + KILL_LEFT

#define GIFT_LEFT_DIST -96
#define GIFT_RIGHT_DIST 32

#define MIN_DIST 150
#define MAX_DIST 200

#define GROUND 8

// returns x
static float renew_house(Houses *self, int renew_idx, float farest_x_pos) {
    float x = farest_x_pos;
    x += sca_random_range(MIN_DIST, MAX_DIST);
    
    rRect_s *house = &self->L.ro.rects[renew_idx];
    rRect_s *gifts = &self->L.gifts.rects[renew_idx*3];
    
    house->pose = u_pose_new(x, 64+ GROUND, 128, 128);
    house->sprite.x = 1;
    house->color.rgb = vec3_random_range(0.8, 1.0);
    
    char buf[4];
    snprintf(buf, 4, "%3i", ++self->L.farest_number);
    ro_text_set_text(&self->L.number[renew_idx], buf);
    self->L.number[renew_idx].pose = u_pose_new(
            x - 64+2, 
            32 + GROUND, 
            1, 1);
    
    int gnum = 1 + rand()%3;
    
    for(int i=0; i<gnum; i++) {
        float gx, gy;
        if(i==0) {
            gx = x+8;
            gy = 48;
        } else if (i==1) {
            gx = x+24;
            gy = 48;
        } else {
            gx = x+16;
            gy = 32;
        }
        gifts[i].pose = u_pose_new(gx, gy + GROUND, 16, 16);
        
        int gid = rand()%8;
        gifts[i].sprite.x = gid%4;
        gifts[i].sprite.y = gid/4;
    }
    for(int i=gnum; i<3; i++) {
        gifts[i].pose.m00 = NAN;
    }
    
    return x;
}

Houses *houses_new(PixelParticles *particles) {
    Houses *self = rhc_calloc(sizeof *self);
    
    self->particles_ref = particles;
    
    self->L.ro = ro_batch_new(HOUSES_MAX, r_texture_new_file(2, 1, "res/house.png"));
    
    self->L.gifts = ro_batch_new(HOUSES_MAX*3, r_texture_new_file(4, 2, "res/gifts.png"));
    
    
    for(int i=0; i<HOUSES_MAX; i++) {
        self->L.number[i] = ro_text_new_font55(3);
        ro_text_set_color(&self->L.number[i], R_COLOR_BLACK);
    }
    
    
    float pos = renew_house(self, 0, 200);
    for(int i=1; i<HOUSES_MAX; i++) {
        pos = renew_house(self, i, pos);
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
    float last_x = u_pose_get_x(self->L.ro.rects[self->L.last].pose);
    if(last_x <= self->in.santa_pos + KILL_LEFT) {
        int far_idx = self->L.last-1;
        if(far_idx < 0)
            far_idx = HOUSES_MAX-1;
        float far_x = u_pose_get_x(self->L.ro.rects[far_idx].pose);
        renew_house(self, self->L.last, far_x);
        self->L.last++;
        if(self->L.last>=HOUSES_MAX)
            self->L.last = 0;
    }
    
    float next_x = u_pose_get_x(self->L.ro.rects[self->L.next].pose);
    
    if(next_x < self->in.santa_pos + GIFT_LEFT_DIST) {
         // turn off lights
        self->L.ro.rects[self->L.next].sprite.x = 1;
        
        // todo missing score
        
        self->L.next++;
        if(self->L.next>=HOUSES_MAX)
            self->L.next = 0;
    } else if(next_x < self->in.santa_pos + GIFT_RIGHT_DIST) {
        // turn on lights
        self->L.ro.rects[self->L.next].sprite.x = 0;
        
        // todo check for gifts
        
        
    }
}

void houses_render(const Houses *self, const mat4 *cam_mat) {
    ro_batch_render(&self->L.ro, cam_mat, true);
    ro_batch_render(&self->L.gifts, cam_mat, true);
    for(int i=0; i<HOUSES_MAX; i++) {
        ro_text_render(&self->L.number[i], cam_mat);
    }
}

