#include "r/ro_single.h"
#include "r/texture.h"
#include "u/pose.h"

#include "rhc/alloc.h"
#include "mathc/float.h"
#include "mathc/utils/float.h"

#include "santa.h"

#include "e/gui.h"

#define ANIMATION_FPS 8

#define START_SPEED 32
#define MINUTE_SPEED 128

#define PARTICLES_PS 48
#define PARTICLES_SIZE 2.0


static void emit_particles(Santa *self, int n) {
    if(n>128) n = 128;
    rParticleRect_s p[128] = {0};
    for(int i=0; i<n; i++) {
        p[i].pose = u_pose_new(
                sca_random_noise(self->out.center_pos.x, 48),
                sca_random_noise(self->out.center_pos.y-24, 4),
                PARTICLES_SIZE, PARTICLES_SIZE);
                
        p[i].speed.x = sca_random_noise(-self->speed/4, self->speed/4);
        p[i].speed.y = sca_random_noise(-32, 16);
        
        p[i].acc.y = -p[i].speed.y/2;
        
        p[i].color = (vec4) {{0.8, 0.8, 0.0, 1.0}};
        p[i].color.rgb = vec3_random_noise_vec(p[i].color.rgb, 
                (vec3){{0.2, 0.2, 0.0}});
                
        p[i].color_speed.a = -1;
        
        p[i].start_time = self->particles_ref->time;
    }
    pixelparticles_add(self->particles_ref, p, n);
}


Santa *santa_new(PixelParticles *particles) {
    Santa *self = rhc_calloc(sizeof *self);
    
    self->particles_ref = particles;
    
    self->x = 0;
    self->speed = START_SPEED;
    
    self->game_running = true;
    
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
    
    if(self->game_running) {
        self->speed += dtime * (MINUTE_SPEED-START_SPEED) / 60;
        self->x += dtime * self->speed;
    }
    
    self->out.center_pos = (vec2) {{self->x, 200}};
    self->out.gift_pos = (vec2) {{self->x-32, 200-8}};
    
    self->L.ro.rect.pose = u_pose_new(self->x, 200,
            -128, 64);
            
    self->L.particles_time+=dtime*PARTICLES_PS;
    float particles = (int) self->L.particles_time;
    self->L.particles_time -= particles;
    emit_particles(self, (int) particles);
}

void santa_render(const Santa *self, const mat4 *cam_mat) {
    ro_single_render(&self->L.ro, cam_mat);
}
