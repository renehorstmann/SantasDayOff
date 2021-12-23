#include <SDL2/SDL_mixer.h>
#include "rhc/log.h"
#include "rhc/alloc.h"
#include "mathc/sca/float.h"
#include "sound.h"

#define FALL_PER_SECOND 1.5
#define TOCK_PER_SECOND 1.5

struct Sound {
    eInput *input_ref;

    Mix_Chunk *fall;
    Mix_Chunk *tock;
    Mix_Chunk *fired;

    float fall_per_second;
    float tock_per_second;

    bool active;
};


static void init(Sound *self) {
    
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        log_warn("sound not working");
        return;;
    }

    self->fall = Mix_LoadWAV("res/sound_falldown.wav");
    if (!self->fall) {
        log_warn("failed to load fall");
        return;
    }

    self->tock = Mix_LoadWAV("res/sound_tock.wav");
    if (!self->tock) {
        log_warn("failed to load tock");
        return;
    }

    self->fired = Mix_LoadWAV("res/sound_fired.wav");
    if (!self->fired) {
        log_warn("failed to load fired");
        return;
    }

    Mix_Music *bubbles = Mix_LoadMUS("res/music_christmas_tree.ogg");
    if (!bubbles) {
        log_warn("failed to load music: %s", Mix_GetError());
        return;
    }

    if (Mix_PlayMusic(bubbles, -1) == -1) {
        log_warn("failed to play");
        return;
    }

    log_info("sound activated");
    self->active = true;
}

static void pointer_event(ePointer_s pointer, void *user_data) {
    Sound *self = user_data;

    // wait for first user pointer action

    // init SDL_Mixer
    // on web, sound will be muted, if created before an user action....
    init(self);
    
    e_input_unregister_pointer_event(self->input_ref, pointer_event);
}


Sound *sound_new(eInput *input) {
    Sound *self = rhc_calloc(sizeof *self);
    self->input_ref = input;

    e_input_register_pointer_event(input, pointer_event, self);

    return self;
}

void sound_update(Sound *self, float dtime) {
    if (!self->active)
        return;
    self->fall_per_second = sca_max(0, self->fall_per_second - dtime * FALL_PER_SECOND);
    self->tock_per_second = sca_max(0, self->tock_per_second - dtime * TOCK_PER_SECOND);
}

void sound_play_fall(Sound *self) {
    if (!self->active)
        return;
    if (self->fall_per_second > FALL_PER_SECOND)
        return;
    self->fall_per_second += 1;
    if (Mix_PlayChannel(-1, self->fall, 0) == -1) {
        log_warn("failed to play");
        return;
    }
}

void sound_play_tock(Sound *self) {
    if (!self->active)
        return;
    if (self->tock_per_second > TOCK_PER_SECOND)
        return;
    self->tock_per_second += 1;
    if (Mix_PlayChannel(-1, self->tock, 0) == -1) {
        log_warn("failed to play");
        return;
    }
}

void sound_play_fired(Sound *self) {
    if (!self->active)
        return;
    if (Mix_PlayChannel(-1, self->fired, 0) == -1) {
        log_warn("failed to play");
        return;
    }
}
