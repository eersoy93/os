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

        // Example: draw a line from (20, 20) to (100, 100) with color 0x00FF00 (green)
        putline(state.fbp, &state.vinfo, &state.finfo, 20, 20, 100, 100, 0x00, 0xFF, 0x00);

        // Sleep for a while to avoid busy waiting
        sleep(1);
    }
}
