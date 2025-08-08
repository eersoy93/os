#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "init.h"
#include "gfx.h"
#include "input.h"

void HandleSIGINT(int sig)
{
    (void)sig;
    puts("\nReceived SIGINT, exiting gracefully...");
    exit(EXIT_SUCCESS);
}

SYSTEM_STATE InitTheOS(void)
{
    SYSTEM_STATE state = {0};

    // Disable cursor on tty0 (if available)
    int tty_fd_cursor = open("/dev/tty0", O_WRONLY);
    if (tty_fd_cursor != -1)
    {
        dprintf(tty_fd_cursor, "\033[?25l");
        fsync(tty_fd_cursor);
        close(tty_fd_cursor);
    }

    // Set up signal handler for SIGINT
    signal(SIGINT, HandleSIGINT);

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

void PowerOffTheInit(void)
{
    if (getpid() == 1)
    {
        execl("/bin/poweroff", "poweroff", NULL);
        while (1) pause();
    }
}

LAYOUT ComputeLayout(const SYSTEM_STATE *state)
{
    LAYOUT L;
    L.w = state->vinfo.xres;
    L.h = state->vinfo.yres;
    L.min_dim = (L.w < L.h) ? L.w : L.h;
    L.m = (L.min_dim > 0) ? (L.min_dim / 20) : 10;
    L.r_small = (L.min_dim > 0) ? (L.min_dim / 12) : 15;
    L.r_big = (L.min_dim > 0) ? (L.min_dim / 10) : 20;
    return L;
}

void DisableTerminalEcho(struct termios *out_old)
{
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    if (out_old) *out_old = oldt;
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void RestoreTerminalEcho(const struct termios *oldp)
{
    if (!oldp) return;
    tcsetattr(STDIN_FILENO, TCSANOW, oldp);
}

void DrawInitialScreen(SYSTEM_STATE *state, const LAYOUT *L)
{
    // Draw a pixel near top-left corner (green)
    PutPixel(state->fbp, &state->vinfo, &state->finfo, L->m, L->m, 0x00, 0xFF, 0x00);

    // Draw a line from near top-left towards screen center (white)
    PutLine(state->fbp, &state->vinfo, &state->finfo,
            2 * L->m, 2 * L->m,
            (L->w / 2) - L->m, (L->h / 2) - L->m,
            0xFF, 0xFF, 0xFF);

    // Draw a rectangle outline in the top-right area (red)
    PutBox(state->fbp, &state->vinfo, &state->finfo,
           (L->w * 60) / 100, (L->h * 10) / 100,
           (L->w * 90) / 100, (L->h * 25) / 100,
           0xFF, 0x00, 0x00);

    // Draw a filled rectangle in the left-middle area (blue)
    PutBoxFilled(state->fbp, &state->vinfo, &state->finfo,
                 (L->w * 10) / 100, (L->h * 60) / 100,
                 (L->w * 30) / 100, (L->h * 80) / 100,
                 0x00, 0x00, 0xFF);

    // Draw a circle outline in the upper-left quadrant (yellow)
    PutCircle(state->fbp, &state->vinfo, &state->finfo,
              (L->w * 30) / 100, (L->h * 30) / 100,
              L->r_small,
              0xFF, 0xFF, 0x00);

    // Draw a filled circle in the bottom-right quadrant (magenta)
    PutCircleFilled(state->fbp, &state->vinfo, &state->finfo,
                    (L->w * 75) / 100, (L->h * 75) / 100,
                    L->r_big,
                    0xFF, 0x00, 0xFF);

    // Flush the framebuffer to apply changes
    FlushTheFramebuffer(state->fbp, &state->vinfo, &state->finfo);
}

void CleanupTheSytem(SYSTEM_STATE *state)
{
    if (state->tty_fd != -1)
    {
        dprintf(state->tty_fd, "\033[?25h"); // Show cursor
        fsync(state->tty_fd);
        close(state->tty_fd);
        state->tty_fd = -1;
    }
    if (state->fbp != MAP_FAILED)
    {
        munmap(state->fbp, state->vinfo.yres_virtual * state->finfo.line_length);
        state->fbp = MAP_FAILED;
    }
    if (state->fb_fd != -1)
    {
        close(state->fb_fd);
        state->fb_fd = -1;
    }
}
