#ifndef INIT_H
#define INIT_H

#include <linux/fb.h>
#include <stdint.h>

struct system_state {
    int tty_fd;
    uint8_t *fbp;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
};

struct system_state init_system(void);

#endif // INIT_H
