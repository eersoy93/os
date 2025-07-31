#ifndef GFX_H
#define GFX_H

#include <stdint.h>
#include <linux/fb.h>

// Draws a pixel at (x, y) with the specified RGB color
void putpixel(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int x, int y, uint8_t r, uint8_t g, uint8_t b);

// Draws a line from (x0, y0) to (x1, y1) with the specified RGB color
void putline(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b);

// Draws a rectangle outline from (x0, y0) to (x1, y1) with the specified RGB color
void putbox(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b);

// Draws a filled rectangle from (x0, y0) to (x1, y1) with the specified RGB color
void putboxfilled(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b);

// Draws a circle outline at (xc, yc) with the specified radius and RGB color
void putcircle(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int xc, int yc, int radius, uint8_t r, uint8_t g, uint8_t b);

// Draws a filled circle at (xc, yc) with the specified radius and RGB color
void putcirclefilled(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int xc, int yc, int radius, uint8_t r, uint8_t g, uint8_t b);

#endif // GFX_H