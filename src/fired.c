#include "r/ro_single.h"
#include "r/ro_text.h"
#include "r/texture.h"
#include "u/pose.h"
#include "rhc/log.h"
#include "mathc/float.h"
#include "button.h"
#include "fired.h"


#define METER_SHOW 0.25

#define TEXT_SIZE 2.0


Fired *fired_new(const char *name_ref) {
    Fired *self = rhc_calloc(sizeof *self);
    
    self->name_ref = name_ref;

    self->L.ro = ro_single_new(r_texture_new_white_pixel());
    self->L.ro.rect.pose = u_pose_new(0, 0, 2048, 2048);
    self->L.ro.rect.color = (vec4) {{0.5, 0.5, 0.5, 0.0}};

    self->L.btn = ro_single_new(r_texture_new_file(2, 1, "res/retry.png"));

    self->L.info = ro_text_new_font55(64);
    ro_text_set_color(&self->L.info, (vec4) {{0.8, 0.1, 0.1, 1.0}});

    self->L.credits = ro_text_new_font55(64);
    ro_text_set_text(&self->L.credits, "\"Santas Day Off\" by horsimann");

    return self;
}

void fired_kill(Fired **self_ptr) {
    Fired *self = *self_ptr;
    if (!self)
        return;
    showscore_kill(&self->showscore);
    ro_single_kill(&self->L.ro);
    ro_text_kill(&self->L.info);
    ro_single_kill(&self->L.btn);
    ro_text_kill(&self->L.credits);
    rhc_free(self);
    *self_ptr = NULL;
}

void fired_update(Fired *self, float dtime, const Camera_s *cam) {
    

    char buf[65];
    if (self->in.meter>0 && self->in.meter<METER_SHOW) {
        sprintf(buf, "Warning!");
    } else if(self->in.meter <= 0) {
        sprintf(buf, "You are fired!");
        
        if(!self->showscore) {
            log_info("fired: starting showscore");
            self->showscore = showscore_new(self->name_ref, self->in.score);
        }
    }

    vec2 size = ro_text_set_text(&self->L.info, buf);
    self->L.info.pose = u_pose_new(
            sca_floor(-size.x / 2 * TEXT_SIZE),
            cam->RO.top-40,
            TEXT_SIZE, TEXT_SIZE);

    self->L.ro.rect.color.a = sca_mix(0.5, 0.25, self->in.meter/METER_SHOW);

    self->L.btn.rect.pose = u_pose_new(0, -20-64, 64, 64);

    self->L.credits.pose = u_pose_new_aa(
            cam->RO.left + 10, 
            cam->RO.top, 
            1, 1);
    
    if(self->showscore) {
        self->showscore->in.pos = (vec2) {{-115, cam->RO.top - 60}};
        showscore_update(self->showscore, dtime);
    }
}

void fired_render(const Fired *self, const mat4 *cam_mat) {
    
    if (self->in.meter > METER_SHOW)
        return;
    ro_single_render(&self->L.ro, cam_mat);
    ro_text_render(&self->L.info, cam_mat);
    if(self->showscore)
        showscore_render(self->showscore, cam_mat);    
    if (self->in.meter <= 0) {
        ro_single_render(&self->L.btn, cam_mat);
        ro_text_render(&self->L.credits, cam_mat);
    }
}

void fired_pointer_event(Fired *self, ePointer_s pointer) {
    if (self->in.meter > METER_SHOW)
        return;

    if(self->showscore)
        showscore_pointer(self->showscore, pointer);

    if (button_clicked(&self->L.btn.rect, pointer)) {
        self->out.reset_game = true;
    }
}
