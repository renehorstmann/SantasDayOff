#ifndef SANTASDAYOFF_BACKGROUND_H
#define SANTASDAYOFF_BACKGROUND_H

#include "r/render.h"
#include "r/ro_types.h"
#include "camera.h"

typedef struct {
    struct {
        RoBatch ro[CAMERA_BACKGROUNDS];
    } L;
} Background;


Background *background_new(rRender *render, float level_width, float level_height,
                           bool repeat_h, bool repeat_v,
                           const char *file);

void background_kill(Background **self_ptr);

void background_render(const Background *self, const Camera_s *cam);

#endif //SANTASDAYOFF_BACKGROUND_H
