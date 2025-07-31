#include "init.h"
#include <unistd.h>

int main(void)
{
    struct system_state state = init_system();

    // Main loop
    while (1)
    {
        // Draw a green point at (row=10, col=20) on tty0
        if (state.tty_fd != -1)
        {
            dprintf(state.tty_fd, "\033[2;1H\033[32m\033[0m");
            fsync(state.tty_fd);
        }

        // Draw a green point at (100, 100)
        int x = 100, y = 100;
        long location = (x + state.vinfo.xoffset) * (state.vinfo.bits_per_pixel / 8) +
                        (y + state.vinfo.yoffset) * state.finfo.line_length;
        if (state.vinfo.bits_per_pixel == 32)
        {
            // ARGB or BGRA, set green pixel
            state.fbp[location + 0] = 0x00; // Blue
            state.fbp[location + 1] = 0xFF; // Green
            state.fbp[location + 2] = 0x00; // Red
            state.fbp[location + 3] = 0x00; // Alpha
        }
        else if (state.vinfo.bits_per_pixel == 24)
        {
            state.fbp[location + 0] = 0x00; // Blue
            state.fbp[location + 1] = 0xFF; // Green
            state.fbp[location + 2] = 0x00; // Red
        }

        sleep(1);
    }

    // Cleanup (unreachable)
    return 0;
}
