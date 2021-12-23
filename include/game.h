#ifndef SANTASDAYOFF_GAME_H
#define SANTASDAYOFF_GAME_H

#include "e/input.h"
#include "camera.h"

#include "pixelparticles.h"

#include "santa.h"
#include "bag.h"
#include "gifts.h"
#include "houses.h"
#include "score.h"
#include "fired.h"

typedef struct Game {
    PixelParticles *particles_ref;
    Sound *sound_ref;
    
    const char *name_ref;


    Santa *santa;
    Bag *bag;
    Gifts *gifts;
    Houses *houses;
    Score *score;
    Fired *fired;

} Game;


Game *game_new(PixelParticles *particles, Sound *sound, const char *name_ref);

void game_update(Game *self, float dtime, Camera_s *cam);

void game_render_main(const Game *self, const mat4 *cam_mat);

void game_render_hud(const Game *self, const mat4 *hudcam_mat);

void game_reset(Game *self);

void game_pointer_event(Game *self, ePointer_s pointer);

#endif //SANTASDAYOFF_GAME_H
