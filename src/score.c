#include "r/ro_single.h"
#include "r/ro_text.h"
#include "r/texture.h"
#include "u/pose.h"

#include "rhc/alloc.h"
#include "mathc/sca/float.h"

#include "score.h"

#include "e/gui.h"


#define RED_ZONE 0.20
#define YELLOW_ZONE 0.45

#define METER_RIGHT +0.1
#define METER_WRONG -0.05
#define METER_MISSED -0.1

Score *score_new() {
    Score *self = rhc_calloc(sizeof *self);
    
    self->meter = 0.5;
    self->score = 0;
    
    self->L.meter_ro = ro_single_new(r_texture_new_file(1, 3, "res/santaometer.png"));
    
    self->L.box_ro = ro_single_new(r_texture_new_white_pixel());
    
    self->L.score_ro = ro_text_new_font55(64);
    
    return self;
}

void score_kill(Score **self_ptr) {
    Score *self = *self_ptr;
    if(!self)
        return;
    ro_single_kill(&self->L.meter_ro);
    ro_single_kill(&self->L.box_ro);
    ro_text_kill(&self->L.score_ro);
    rhc_free(self);
    *self_ptr = NULL;
}

void score_update(Score *self, float dtime, const Camera_s *cam) {
    
    // todo meter + score
    self->meter += self->in.right_gifts*METER_RIGHT
            + self->in.wrong_gifts*METER_WRONG
            + self->in.missed_gifts*METER_MISSED;
    
    self->meter = sca_min(1, sca_max(0, self->meter));
    
    self->score += self->in.right_gifts * 100 * (self->meter+1);
    
    self->L.meter_ro.rect.pose = u_pose_new_aa(cam->RO.left+24, cam->RO.top-8, 96, 24);
    
    self->L.box_ro.rect.pose = u_pose_new_aa(cam->RO.left +24 + 24, cam->RO.top-8-1, 
            self->meter*54, 
            8);
    
    if(self->meter <= RED_ZONE) {
        self->L.meter_ro.rect.sprite.y = 0;
        self->L.box_ro.rect.color = (vec4) {{
            1.0, 0.0, 0.0, 1.0
        }};
    } else if(self->meter <= YELLOW_ZONE) {
        self->L.meter_ro.rect.sprite.y = 1;
        self->L.box_ro.rect.color = (vec4) {{
            1.0, 1.0, 0.0, 1.0
        }};
    } else {
        self->L.meter_ro.rect.sprite.y = 2;
        self->L.box_ro.rect.color = (vec4) {{
            0.0, 1.0, 0.0, 1.0
        }};
    }
    
    char buf[64];
    snprintf(buf, sizeof buf, "Score: %-9i", self->score);
    vec2 size = ro_text_set_text(&self->L.score_ro, buf);
    
    self->L.score_ro.pose = u_pose_new_aa(cam->RO.right - size.x, cam->RO.top-16, 1, 1);
    
}

void score_render(const Score *self, const mat4 *cam_mat) {
    ro_single_render(&self->L.box_ro, cam_mat);
    ro_single_render(&self->L.meter_ro, cam_mat);
    ro_text_render(&self->L.score_ro, cam_mat);
}

