#ifndef GFX_H
#define GFX_H

#include <stdint.h>
#include <linux/fb.h>

void putpixel(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int x, int y, uint8_t r, uint8_t g, uint8_t b);
void putline(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b);

#endif // GFX_H