#include <stdio.h>
#include <unistd.h>
#include "init.h"
#include "gfx.h"

int main(void)
{
    struct system_state state = init_system();
    
    // Main loop
    while (1)
    {
        // Example: draw a pixel at (10, 10) with color 0x00FF00 (green)
        putpixel(state.fbp, &state.vinfo, &state.finfo, 10, 10, 0x00, 0xFF, 0x00);

        // Example: draw a line from (20, 20) to (100, 100) with color 0xFFFFFF (white)
        putline(state.fbp, &state.vinfo, &state.finfo, 20, 20, 100, 100, 0xFF, 0xFF, 0xFF);

        // Example: draw a rectangle outline from (50, 50) to (150, 100) in red
        putbox(state.fbp, &state.vinfo, &state.finfo, 50, 50, 150, 100, 0xFF, 0x00, 0x00);

        // Example: draw a filled rectangle from (160, 50) to (260, 100) in blue
        putboxfilled(state.fbp, &state.vinfo, &state.finfo, 160, 50, 260, 100, 0x00, 0x00, 0xFF);

        // Example: draw a circle outline at (80, 180) with radius 30 in yellow
        putcircle(state.fbp, &state.vinfo, &state.finfo, 80, 180, 30, 0xFF, 0xFF, 0x00);

        // Example: draw a filled circle at (200, 180) with radius 30 in magenta
        putcirclefilled(state.fbp, &state.vinfo, &state.finfo, 200, 180, 30, 0xFF, 0x00, 0xFF);

        // Sleep for a while to avoid busy waiting
        sleep(1);
    }
}
