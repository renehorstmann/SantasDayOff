#include "rhc/alloc.h"
#include "mathc/sca/float.h"
#include "game.h"


static void game_start(Game *self) {
    self->santa = santa_new(self->particles_ref);
    self->bag = bag_new(); 
    self->gifts = gifts_new(self->particles_ref);
    self->houses = houses_new(self->particles_ref); 
    self->score = score_new();
    self->fired = fired_new(self->name_ref);
}

static void game_end(Game *self) {
    santa_kill(&self->santa);
    bag_kill(&self->bag);
    gifts_kill(&self->gifts);
    houses_kill(&self->houses);
    score_kill(&self->score);
    fired_kill(&self->fired);
}

Game *game_new(PixelParticles *particles, const char *name_ref) {
    Game *self = rhc_calloc(sizeof *self);

    self->particles_ref = particles;
    
    self->name_ref = name_ref;

    game_start(self);

    return self;
}

void game_update(Game *self, float dtime, Camera_s *cam) {
    santa_update(self->santa, dtime);
    
    bag_update(self->bag, dtime, cam);
    
    self->gifts->in.start_speed = -2*self->santa->speed;
    gifts_update(self->gifts, dtime);
    
    
    float pos = self->santa->out.center_pos.x;
    pos += 120-cam->RO.right;
    
    float x_min = -cam->RO.left;
    float y = -cam->RO.bottom;
    camera_set_pos(cam, sca_max(pos, x_min), y);
    
    for(int i=0; i<8; i++) {
        if(self->bag->out.pressed[i]) {
            gifts_add(self->gifts, i, self->santa->out.gift_pos);
        }
    }
    
    self->houses->in.santa_pos = self->santa->out.center_pos.x;
    memcpy(self->houses->in.gifts, self->bag->out.pressed, sizeof self->houses->in.gifts);
    houses_update(self->houses, dtime);
    
    
    self->score->in.missed_gifts = self->houses->out.missed_gifts;
    self->score->in.right_gifts = self->houses->out.right_gifts;
    self->score->in.wrong_gifts = self->houses->out.wrong_gifts;
    score_update(self->score, dtime, cam);
    
    if(self->score->meter <= 0)
        self->santa->game_running = false;
        
    self->fired->in.meter = self->score->meter;
    self->fired->in.score = self->score->score;
    fired_update(self->fired, dtime, cam);
    if(self->fired->out.reset_game) {
        game_reset(self);
    }
}

void game_render_main(const Game *self, const mat4 *cam_mat) {
    houses_render(self->houses, cam_mat); 
    gifts_render(self->gifts, cam_mat);
    santa_render(self->santa, cam_mat);
}

void game_render_hud(const Game *self, const mat4 *hudcam_mat) {
    bag_render(self->bag, hudcam_mat);
    fired_render(self->fired, hudcam_mat);
    score_render(self->score, hudcam_mat);
}


void game_reset(Game *self) {
    game_end(self);
    game_start(self);
}

void game_pointer_event(Game *self, ePointer_s pointer) {
    if(self->score->meter>0)
        bag_pointer_event(self->bag, pointer);
    fired_pointer_event(self->fired, pointer);
}
