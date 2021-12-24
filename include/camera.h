#ifndef SANTASDAYOFF_CAMERA_H
#define SANTASDAYOFF_CAMERA_H

/*
 * PixelPerfect canvas camera with view matrix.
 * To control the camera position and size
 */

#include <stdbool.h>
#include "mathc/types/float.h"
#include "mathc/types/int.h"


#define CAMERA_SIZE 240 // *4=720; *6=1080; *8=1440
#define CAMERA_SCREEN_WEIGHT (4.0/5.0)
#define CAMERA_BACKGROUNDS 6

struct CameraMatrices_s {
    mat4 v;
    mat4 v_inv;
    mat4 vp;
    mat4 v_p_inv;   // v @ p_inv
};

typedef struct {
    mat4 matrices_p;
    mat4 matrices_p_inv;

    struct CameraMatrices_s matrices_background[CAMERA_BACKGROUNDS];
    struct CameraMatrices_s matrices_main;
    
    
    struct {
        float scale;    // units per pixel
        float left, right, bottom, top;
        float full_left, full_right, full_bottom, full_top;
        
        vec2 offset;
        
        // in texture space (origin is top left) [0:1]
        // as center_x, _y, radius_x, _y
        vec4 view_aabb;
    } RO; // read only

} Camera_s;


Camera_s camera_new();

void camera_update(Camera_s *self, ivec2 window_size);


static float camera_width(const Camera_s *self) {
    return -self->RO.left + self->RO.right;
}

static float camera_height(const Camera_s *self) {
    return -self->RO.bottom + self->RO.top;
}

static float camera_full_width(const Camera_s *self) {
    return -self->RO.full_left + self->RO.full_right;
}

static float camera_full_height(const Camera_s *self) {
    return -self->RO.full_bottom + self->RO.full_top;
}


void camera_set_pos(Camera_s *self, float x, float y);

void camera_set_size(Camera_s *self, float size);

void camera_set_angle(Camera_s *self, float alpha);

static bool camera_is_portrait_mode(const Camera_s *self) {
    return camera_full_height(self) > camera_full_width(self);
}


#endif //SANTASDAYOFF_CAMERA_H
