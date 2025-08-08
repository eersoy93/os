#ifndef INIT_H
#define INIT_H

#include <linux/fb.h>
#include <stdint.h>
#include <termios.h>
#include "gfx.h"

// System state structure to hold framebuffer and terminal information
typedef struct SYSTEM_STATE_TYPE
{
    int tty_fd;               // File descriptor for tty0
    int fb_fd;                // File descriptor for framebuffer
    uint8_t *fbp;             // Pointer to framebuffer memory
    struct fb_var_screeninfo vinfo; // Variable screen info
    struct fb_fix_screeninfo finfo; // Fixed screen info
} SYSTEM_STATE;

SYSTEM_STATE InitTheOS(void);

// Helper functions used by main
LAYOUT ComputeLayout(const SYSTEM_STATE *state);
void DisableTerminalEcho(struct termios *out_old);
void RestoreTerminalEcho(const struct termios *oldp);
void DrawInitialScreen(SYSTEM_STATE *state, const LAYOUT *L);
void CleanupTheSytem(SYSTEM_STATE *state);
void PowerOffTheInit(void);

#endif // INIT_H
