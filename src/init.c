#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <stdint.h>

void handle_sigint(int sig)
{
    (void)sig;

    puts("\nReceived SIGINT, exiting gracefully...");

    exit(EXIT_SUCCESS);
}

int main(void)
{
    puts("Hello, World!");
    fflush(stdout);

    signal(SIGINT, handle_sigint);

    int fb_fd = open("/dev/fb0", O_RDWR);
    if (fb_fd == -1)
    {
        perror("open framebuffer");
        // Never exit, just wait forever
        while (1) pause();
    }

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    if (ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo) == -1 ||
        ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo) == -1)
    {
        perror("ioctl framebuffer");
        close(fb_fd);
        while (1) pause();
    }

    long screen_size = vinfo.yres_virtual * finfo.line_length;
    uint8_t *fbp = (uint8_t *)mmap(0, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
    if ((intptr_t)fbp == -1)
    {
        perror("mmap framebuffer");
        close(fb_fd);
        while (1) pause();
    }

    // Open /dev/tty0 for drawing
    int tty_fd = open("/dev/tty0", O_WRONLY);
    if (tty_fd == -1)
    {
        perror("open /dev/tty0");
        // Continue running, but skip drawing
    }

    // Main loop
    while (1)
    {
        // Draw a green point at (row=10, col=20) on tty0
        if (tty_fd != -1)
        {
            dprintf(tty_fd, "\033[2;1H\033[32m\033[0m");
            fsync(tty_fd);
        }

        // Draw a green point at (100, 100)
        int x = 100, y = 100;
        long location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
                        (y + vinfo.yoffset) * finfo.line_length;
        if (vinfo.bits_per_pixel == 32)
        {
            // ARGB or BGRA, set green pixel
            fbp[location + 0] = 0x00; // Blue
            fbp[location + 1] = 0xFF; // Green
            fbp[location + 2] = 0x00; // Red
            fbp[location + 3] = 0x00; // Alpha
        }
        else if (vinfo.bits_per_pixel == 24)
        {
            fbp[location + 0] = 0x00; // Blue
            fbp[location + 1] = 0xFF; // Green
            fbp[location + 2] = 0x00; // Red
        }

        sleep(1);
    }

    if (tty_fd != -1) close(tty_fd);
    munmap(fbp, screen_size);
    close(fb_fd);
    return EXIT_SUCCESS;
}
