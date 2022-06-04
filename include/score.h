#ifndef SANTASDAYOFF_SCORE_H
#define SANTASDAYOFF_SCORE_H

#include "r/ro_types.h"
#include "camera.h"

typedef struct {
    
    float meter;
    int score;
    
    struct {
        int missed_gifts;
        int right_gifts;
        int wrong_gifts;
    } in;
    
    struct {
        RoSingle meter_ro;
        RoSingle box_ro;
        RoText score_ro;
    } L;
} Score;


Score *score_new();

void score_kill(Score **self_ptr);

void score_update(Score *self, float dtime, const Camera_s *cam);

void score_render(const Score *self, const mat4 *cam_mat);

#endif //SANTASDAYOFF_SCORE_H
