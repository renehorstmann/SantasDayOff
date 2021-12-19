#include "e/simple.h"

// hello world
#include "r/ro_text.h"
#include "u/pose.h"

#include "camera.h"
#include "background.h"
#include "snow.h"
#include "santa.h"
#include "bag.h"
#include "gifts.h"

static struct {
    Camera_s camera;
    
    PixelParticles *particles;
    
    Background *bg;
    Snow *snow;
    Santa *santa;
    Bag *bag;
    Gifts *gifts;
} L;


static void pointer_event(ePointer_s pointer, void *ud) {
     // hud pointer.pos in hud coords
    ePointer_s hud_pointer = pointer;
    hud_pointer.pos = mat4_mul_vec(L.camera.matrices_p_inv, pointer.pos);
    
    bag_pointer_event(L.bag, hud_pointer);
}


// this function will be called at the start of the app
static void init(eSimple *simple, ivec2 window_size) {
    
    e_input_register_pointer_event(simple->input, pointer_event, NULL);
    
    // init systems
    L.camera = camera_new();
    
    L.particles = pixelparticles_new();

    L.bg = background_new(simple->render, 1024*64, 1024, true, false, "res/bg.png");
    
    L.snow = snow_new();
    
    L.santa = santa_new(L.particles);
    
    L.bag = bag_new();
    
    L.gifts = gifts_new(L.particles);
}


// this functions is called either each frame or at a specific update/s time
static void update(eSimple *simple, ivec2 window_size, float dtime) {
    // simulate
    
    pixelparticles_update(L.particles, dtime);
    
    santa_update(L.santa, dtime);
    
    bag_update(L.bag, dtime, &L.camera);
    
    L.gifts->in.start_speed = -2*L.santa->speed;
    gifts_update(L.gifts, dtime);
    
    
    float pos = L.santa->out.center_pos.x;
    pos += 0;
    
    
    camera_set_pos(&L.camera, pos, 0);
    
    for(int i=0; i<8; i++) {
        if(L.bag->out.pressed[i]) {
            gifts_add(L.gifts, i, L.santa->out.gift_pos);
        }
    }
    
    snow_update(L.snow, dtime, pos - camera_width(&L.camera)/2, pos + camera_width(&L.camera)/2, L.camera.RO.top);
    
}


// this function is calles each frame to render stuff, dtime is the time between frames
static void render(eSimple *simple, ivec2 window_size, float dtime) {
    camera_update(&L.camera, window_size);
    
    mat4 *camera_mat = &L.camera.matrices_main.vp;
    mat4 *hudcam_mat = &L.camera.matrices_p;

    background_render(L.bg, &L.camera);
    
    pixelparticles_render(L.particles, camera_mat);
    
    gifts_render(L.gifts, camera_mat);
    
    santa_render(L.santa, camera_mat);
    
    snow_render(L.snow, camera_mat);
    
    bag_render(L.bag, hudcam_mat);
    
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
