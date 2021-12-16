#ifndef SANTASDAYOFF_CAMERACTRL_H
#define SANTASDAYOFF_CAMERACTRL_H

#include "camera.h"

typedef struct {
    vec2 pos;

    struct {
        vec2 dst;
    } in;
} CameraCtrl_s;

CameraCtrl_s cameractrl_new();

void cameractrl_update(CameraCtrl_s *self, Camera_s *cam, float dtime);

#endif //SANTASDAYOFF_CAMERACTRL_H
