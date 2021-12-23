#include <SDL2/SDL_mixer.h>
#include "rhc/log.h"
#include "rhc/alloc.h"
#include "mathc/sca/float.h"
#include "sound.h"

struct Sound {
    eInput *input_ref;

    Mix_Chunk *fall;
    Mix_Chunk *tock;
    Mix_Chunk *boom;
    Mix_Chunk *fired;

    Mix_Music *song;

    bool active;
};


static void init(Sound *self) {
    
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        log_warn("sound not working");
        return;
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

    self->boom = Mix_LoadWAV("res/sound_boom.wav");
    if (!self->boom) {
        log_warn("failed to load boom");
        return;
    }

    self->fired = Mix_LoadWAV("res/sound_fired.wav");
    if (!self->fired) {
        log_warn("failed to load fired");
        return;
    }

    self->song = Mix_LoadMUS("res/music_christmas_tree.ogg");
    if (!self->song) {
        log_warn("failed to load music: %s", Mix_GetError());
        return;
    }

    Mix_VolumeMusic(64);    // 128 is max



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
}

void sound_start_music(Sound *self) {
    if (Mix_PlayMusic(self->song, -1) == -1) {
        log_warn("failed to play music");
        return;
    }
}

void sound_play_fall(Sound *self) {
    if (!self->active)
        return;
    if (Mix_PlayChannel(-1, self->fall, 0) == -1) {
        log_warn("failed to play");
        return;
    }
}

void sound_play_tock(Sound *self) {
    if (!self->active)
        return;
    if (Mix_PlayChannel(-1, self->tock, 0) == -1) {
        log_warn("failed to play");
        return;
    }
}

void sound_play_boom(Sound *self) {
    if (!self->active)
        return;
    if (Mix_PlayChannel(-1, self->boom, 0) == -1) {
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
