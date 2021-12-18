#include "e/simple.h"

// hello world
#include "r/ro_text.h"
#include "u/pose.h"

#include "camera.h"
#include "background.h"
#include "santa.h"

static struct {
    Camera_s camera;
    
    Background *bg;
    Santa *santa;
} L;


// this function will be called at the start of the app
static void init(eSimple *simple, ivec2 window_size) {
    // init systems
    L.camera = camera_new();

    L.bg = background_new(simple->render, 1024*64, 1024, true, true, "res/bg.png");
    
    L.santa = santa_new(NULL);
}


// this functions is called either each frame or at a specific update/s time
static void update(eSimple *simple, ivec2 window_size, float dtime) {
    // simulate
    camera_update(&L.camera, window_size);
    
    santa_update(L.santa, dtime);
    
    
    static float pos = 150;
    pos += dtime*100;
    
    camera_set_pos(&L.camera, pos, 250);
}


// this function is calles each frame to render stuff, dtime is the time between frames
static void render(eSimple *simple, ivec2 window_size, float dtime) {
    mat4 *camera_mat = &L.camera.matrices_main.vp;

    background_render(L.bg, &L.camera);
    
    santa_render(L.santa, camera_mat);
    
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
