#include "e/simple.h"

// hello world
#include "r/ro_text.h"
#include "u/pose.h"

#include "mathc/sca/float.h"
#include "camera.h"
#include "background.h"
#include "snow.h"
#include "login.h"
#include "game.h"


static struct {
    Camera_s camera;
    
    PixelParticles *particles;
    Sound *sound;
    
    Background *bg;
    Snow *snow;
    
    Login *login;
    Game *game;
    
    char name[HIGHSCORE_NAME_MAX_LENGTH+1];
} L;


static void pointer_event(ePointer_s pointer, void *ud) {
     // hud pointer.pos in hud coords
    ePointer_s hud_pointer = pointer;
    hud_pointer.pos = mat4_mul_vec(L.camera.matrices_p_inv, pointer.pos);
    
    if(L.login)
        login_pointer_event(L.login, hud_pointer);
    
    if(L.game)
        game_pointer_event(L.game, hud_pointer);
}


// this function will be called at the start of the app
static void init(eSimple *simple, ivec2 window_size) {
    
    e_input_register_pointer_event(simple->input, pointer_event, NULL);
    
    // init systems
    L.camera = camera_new();
    
    L.particles = pixelparticles_new();

    L.sound = sound_new(simple->input);

    L.bg = background_new(simple->render, 1024*64, 1024, true, false, "res/bg.png");
    
    L.snow = snow_new();
    
    L.login = login_new(simple->input);
}


// this functions is called either each frame or at a specific update/s time
static void update(eSimple *simple, ivec2 window_size, float dtime) {
    // simulate
    
    pixelparticles_update(L.particles, dtime);
    
    
    float pos = u_pose_get_x(L.camera.matrices_main.v);
    float y;
    
    if(L.login) {
        y = 64-L.camera.RO.full_bottom;
        camera_set_pos(&L.camera, -L.camera.RO.full_left, y);
        login_update(L.login, window_size, dtime);   
         
        if(L.login->out.done) {
            strcpy(L.name, L.login->out.name);
            login_kill(&L.login);
            L.game = game_new(L.particles, L.sound, L.name);
            sound_start_music(L.sound);
        }
    }
    
    if(L.game) {
         y = -L.camera.RO.bottom;
        game_update(L.game, dtime, &L.camera);  
    }
    
    
    snow_update(L.snow, dtime, pos - camera_width(&L.camera)/2, pos + camera_width(&L.camera)/2, L.camera.RO.top + y);
}


// this function is calles each frame to render stuff, dtime is the time between frames
static void render(eSimple *simple, ivec2 window_size, float dtime) {
    camera_update(&L.camera, window_size);
    
    mat4 *camera_mat = &L.camera.matrices_main.vp;
    mat4 *hudcam_mat = &L.camera.matrices_p;

    background_render(L.bg, &L.camera);
    
    pixelparticles_render(L.particles, camera_mat);
    
    if(L.login)
        login_render(L.login, hudcam_mat);
    
    if(L.game)
        game_render_main(L.game, camera_mat);
    
    snow_render(L.snow, camera_mat);
    
    if(L.game)
        game_render_hud(L.game, hudcam_mat);
        
        
    // uncomment to clone the current framebuffer into r_render_get_framebuffer_tex
    // r_render_blit_framebuffer(simple->render, window_size);
}


int main(int argc, char **argv) {
    e_simple_start("Santas Day Off", "Horsimann",
            1.0f,   // startup block time (the time in which "Horsimann" is displayed at startup)
            0,      // updates/s, <=0 to turn off and use fps
            init, update, render);
    return 0;
}
