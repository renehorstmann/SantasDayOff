#ifndef SANTASDAYOFF_BAG_H
#define SANTASDAYOFF_BAG_H

#include "e/input.h"
#include "r/ro_types.h"
#include "camera.h"

typedef struct {
    struct {
        bool pressed[8];
    } out;
    
    struct {
        RoSingle bg;
        RoBatch gifts;
        bool pressed[8];
    } L;
} Bag;


Bag *bag_new();

void bag_kill(Bag **self_ptr);

void bag_update(Bag *self, float dtime, const Camera_s *cam);

void bag_render(const Bag *self, const mat4 *cam_mat);

void bag_pointer_event(Bag *self, ePointer_s pointer);

#endif //SANTASDAYOFF_BAG_H
