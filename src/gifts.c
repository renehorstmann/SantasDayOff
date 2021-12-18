#include "r/ro_batch.h"
#include "r/texture.h"
#include "u/pose.h"

#include "rhc/alloc.h"
#include "mathc/utils/random.h"

#include "gifts.h"

#define START_SPEED -64
#define ACC -100


Gifts *gifts_new(PixelParticles *particles) {
    Gifts *self = rhc_calloc(sizeof *self);
    
    self->particles_ref = particles;
    
    self->L.ro = ro_batch_new(GIFTS_MAX, r_texture_new_file(4, 2, "res/gifts.png"));
    
    for(int i=0; i<GIFTS_MAX; i++) {
        self->L.ro.rects[i].pose = u_pose_new(NAN, NAN, 32, 32);
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
    
    for(int i=0; i<GIFTS_MAX; i++) {
        if(isnan(self->L.speed[i]))
            continue;
        float y = u_pose_get_y(self->L.ro.rects[i].pose);
        self->L.speed[i] += ACC*dtime;
        y += self->L.speed[i] *dtime;
        if(y <= 0) {
            y = 0;
            self->L.speed[i] = NAN;
        }
        
        u_pose_set_y(&self->L.ro.rects[i].pose, y);
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
    self->L.speed[idx] = START_SPEED;
    u_pose_set_xy(&self->L.ro.rects[idx].pose, start.x, start.y);
    self->L.ro.rects[idx].sprite = (vec2) {{ id%4, id/4 }};
    self->L.ro.rects[idx].color.rgb = vec3_random_range(0.75, 1.0);
}
