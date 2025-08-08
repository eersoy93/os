#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
// #include <termios.h> // not needed here anymore

#include "init.h"

static void handle_sigint(int sig)
{
    (void)sig;
    puts("\nReceived SIGINT, exiting gracefully...");
    exit(EXIT_SUCCESS);
}

struct system_state InitTheOS(void)
{
    struct system_state state = {0};

    // Disable cursor on tty0 (if available)
    int tty_fd_cursor = open("/dev/tty0", O_WRONLY);
    if (tty_fd_cursor != -1)
    {
        dprintf(tty_fd_cursor, "\033[?25l");
        fsync(tty_fd_cursor);
        close(tty_fd_cursor);
    }

    // Set up signal handler for SIGINT
    signal(SIGINT, handle_sigint);

    // Open framebuffer device
    int fb_fd = open("/dev/fb0", O_RDWR);
    if (fb_fd == -1)
    {
        perror("open framebuffer");
        while (1) pause();
    }

    // Get framebuffer information
    if (ioctl(fb_fd, FBIOGET_FSCREENINFO, &state.finfo) == -1 ||
        ioctl(fb_fd, FBIOGET_VSCREENINFO, &state.vinfo) == -1)
    {
        perror("ioctl framebuffer");
        close(fb_fd);
        while (1) pause();
    }

    // Get screen size and map framebuffer to memory
    long screen_size = state.vinfo.yres_virtual * state.finfo.line_length;
    state.fbp = (uint8_t *)mmap(0, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
    if ((intptr_t)state.fbp == -1)
    {
        perror("mmap framebuffer");
        close(fb_fd);
        while (1) pause();
    }

    // Open /dev/tty0 for drawing
    state.tty_fd = open("/dev/tty0", O_WRONLY);
    if (state.tty_fd == -1)
    {
        perror("open /dev/tty0");
    }
    else
    {
        // Draw a green point at (row=10, col=20) on tty0 automatically at init
        dprintf(state.tty_fd, "\033[10;20H\033[32m\033[0m");
        fsync(state.tty_fd);
    }

    // Set the framebuffer file descriptor in the system state
    state.fb_fd = fb_fd;

    return state;
}

// ReadKey moved to lib/input.c
