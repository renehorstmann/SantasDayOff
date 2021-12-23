#include "r/ro_batch.h"
#include "r/texture.h"
#include "u/pose.h"

#include "rhc/alloc.h"
#include "mathc/utils/random.h"

#include "gifts.h"

#define ACC -200


#define PARTICLES_PS 96
#define PARTICLES_SIZE 2.0

#define GROUND 20


static void emit_particles(Gifts *self, int n, int id, vec2 pos) {
    if(n>128) n = 128;
    rParticleRect_s p[128] = {0};
    for(int i=0; i<n; i++) {
        p[i].pose = u_pose_new(
                sca_random_normal(pos.x, 6),
                sca_random_normal(pos.y, 2),
                PARTICLES_SIZE, PARTICLES_SIZE);
        
        p[i].speed.x = sca_random_noise(0, 8);        
        p[i].speed.y = sca_random_noise(0, 8);        
        if (id == 0) {
            p[i].color = (vec4) {{0.8, 0.0, 0.0, 1.0}};
        } else if(id == 1 || id == 3) {
            p[i].color = (vec4) {{0.0, 0.0, 0.8, 1.0}};
        } else if(id == 2 || id == 4) {
            p[i].color = (vec4) {{0.0, 0.8, 0.0, 1.0}};
        } else if(id == 5) {
            p[i].color = (vec4) {{0.8, 0.0, 0.5, 1.0}};
        } else if(id == 6) {
            p[i].color = (vec4) {{0.8, 0.3, 0.1, 1.0}};
        } else {
            p[i].color = (vec4) {{0.8, 0.8, 0.8, 1.0}};
        }
        
        p[i].color.rgb = vec3_random_noise_vec(p[i].color.rgb, 
                (vec3){{0.3, 0.3, 0.3}});
                
        p[i].color_speed.a = -2;
        
        p[i].start_time = self->particles_ref->time;
    }
    pixelparticles_add(self->particles_ref, p, n);
}



Gifts *gifts_new(PixelParticles *particles, Sound *sound) {
    Gifts *self = rhc_calloc(sizeof *self);
    
    self->particles_ref = particles;
    self->sound_ref = sound;
    
    self->L.ro = ro_batch_new(GIFTS_MAX, r_texture_new_file(4, 2, "res/gifts.png"));

    self->L.play_boom = false;
    
    for(int i=0; i<GIFTS_MAX; i++) {
        self->L.ro.rects[i].pose = u_pose_new(NAN, NAN, 32, 32);
        self->L.speed[i] = NAN;
    }
    
    return self;
}

void gifts_kill(Gifts **self_ptr) {
    Gifts *self = *self_ptr;
    if(!self)
        return;
    ro_batch_kill(&self->L.ro);
    rhc_free(self);
    *self_ptr = NULL;
}

void gifts_update(Gifts *self, float dtime) {
    self->L.particles_time+=dtime*PARTICLES_PS;
    float particles = (int) self->L.particles_time;
    self->L.particles_time -= particles;
    
    for(int i=0; i<GIFTS_MAX; i++) {
        if(isnan(self->L.speed[i]))
            continue;
        float y = u_pose_get_y(self->L.ro.rects[i].pose);
        self->L.speed[i] += ACC*dtime;
        y += self->L.speed[i] *dtime;
        if(y <= GROUND) {
            y = GROUND;
            self->L.speed[i] = NAN;

            if(self->L.play_boom)
                sound_play_boom(self->sound_ref);
            else
                sound_play_tock(self->sound_ref);
        }
        
        u_pose_set_y(&self->L.ro.rects[i].pose, y);
        
        vec2 pos = u_pose_get_xy(self->L.ro.rects[i].pose);
        int id = self->L.ro.rects[i].sprite.x + self->L.ro.rects[i].sprite.y*4;
        
        //emit_particles(self, (int) particles, id, pos);
    }
}

void gifts_render(const Gifts *self, const mat4 *cam_mat) {
    ro_batch_render(&self->L.ro, cam_mat, true);
}

void gifts_add(Gifts *self, int id, vec2 start) {
    self->L.last++;
    if(self->L.last >= GIFTS_MAX)
        self->L.last = 0;
        
    int idx = self->L.last;
    self->L.speed[idx] = self->in.start_speed;
    u_pose_set_xy(&self->L.ro.rects[idx].pose, start.x, start.y);
    self->L.ro.rects[idx].sprite = (vec2) {{ id%4, id/4 }};
    self->L.ro.rects[idx].color.rgb = vec3_random_range(0.75, 1.0);

    sound_play_fall(self->sound_ref);
}
