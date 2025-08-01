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
