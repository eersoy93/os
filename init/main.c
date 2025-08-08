#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "init.h"
#include "gfx.h"
#include <termios.h>

int main(void)
{
    struct system_state state = init_system();

    // Compute screen dimensions and a few helper sizes (relative layout)
    int w = state.vinfo.xres;
    int h = state.vinfo.yres;
    int min_dim = (w < h) ? w : h;
    int m = (min_dim > 0) ? (min_dim / 20) : 10;          // margin
    int r_small = (min_dim > 0) ? (min_dim / 12) : 15;    // small radius
    int r_big = (min_dim > 0) ? (min_dim / 10) : 20;      // big radius

    // Disable terminal echo
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Main loop
    while (1)
    {
        int key = read_key();
        // Exit on 'ESC' key
        if (key == 27) // ASCII code for ESC
        {
            break;
        }

        // Draw a pixel near top-left corner (green)
        putpixel(state.fbp, &state.vinfo, &state.finfo, m, m, 0x00, 0xFF, 0x00);

        // Draw a line from near top-left towards screen center (white)
        putline(state.fbp, &state.vinfo, &state.finfo,
            2 * m, 2 * m,
            (w / 2) - m, (h / 2) - m,
            0xFF, 0xFF, 0xFF);

        // Draw a rectangle outline in the top-right area (red)
        putbox(state.fbp, &state.vinfo, &state.finfo,
            (w * 60) / 100, (h * 10) / 100,
            (w * 90) / 100, (h * 25) / 100,
            0xFF, 0x00, 0x00);

        // Draw a filled rectangle in the left-middle area (blue)
        putboxfilled(state.fbp, &state.vinfo, &state.finfo,
                (w * 10) / 100, (h * 60) / 100,
                (w * 30) / 100, (h * 80) / 100,
                0x00, 0x00, 0xFF);

        // Draw a circle outline in the upper-left quadrant (yellow)
        putcircle(state.fbp, &state.vinfo, &state.finfo,
            (w * 30) / 100, (h * 30) / 100,
            r_small,
            0xFF, 0xFF, 0x00);

        // Draw a filled circle in the bottom-right quadrant (magenta)
        putcirclefilled(state.fbp, &state.vinfo, &state.finfo,
                (w * 75) / 100, (h * 75) / 100,
                r_big,
                0xFF, 0x00, 0xFF);

        // Sleep for a while to avoid busy waiting
        sleep(1);
    }

    // Clean up resources
    if (state.tty_fd != -1)
    {
        dprintf(state.tty_fd, "\033[?25h"); // Show cursor
        fsync(state.tty_fd);
        close(state.tty_fd);
    }
    if (state.fbp != MAP_FAILED)
    {
        munmap(state.fbp, state.vinfo.yres_virtual * state.finfo.line_length);
    }
    close(state.fb_fd);

    // Power off the system if running as init (PID 1)
    if (getpid() == 1)
    {
        execl("/bin/poweroff", "poweroff", NULL);
        while (1) pause();
    }

    return EXIT_SUCCESS;
}
