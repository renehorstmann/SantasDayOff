#ifndef SANTASDAYOFF_SOUND_H
#define SANTASDAYOFF_SOUND_H

#include "e/input.h"

typedef struct Sound Sound;

Sound *sound_new(eInput *input);

void sound_update(Sound *self, float dtime);

void sound_play_fall(Sound *self);

void sound_play_tock(Sound *self);

void sound_play_boom(Sound *self);

void sound_play_fired(Sound *self);

#endif //SANTASDAYOFF_SOUND_H
