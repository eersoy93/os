#ifndef GFX_H
#define GFX_H

#include <stdint.h>
#include <linux/fb.h>
#include <sys/mman.h>

// Layout information for drawing and positioning elements on screen
typedef struct LAYOUT_TYPE
{
    int x, y;       // Top-left corner coordinates
    int w, h;       // Width and height of the layout
    int min_dim;    // Minimum dimension (width or height)
    int m;          // Margin for spacing
    int r_small;    // Radius for small circles
    int r_big;      // Radius for big circles
} LAYOUT;

// Draws a pixel at (x, y) with the specified RGB color
void PutPixel(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int x, int y, uint8_t r, uint8_t g, uint8_t b);

// Draws a line from (x0, y0) to (x1, y1) with the specified RGB color
void PutLine(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b);

// Draws a rectangle outline from (x0, y0) to (x1, y1) with the specified RGB color
void PutBox(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b);

// Draws a filled rectangle from (x0, y0) to (x1, y1) with the specified RGB color
void PutBoxFilled(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b);

// Draws a circle outline at (xc, yc) with the specified radius and RGB color
void PutCircle(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int xc, int yc, int radius, uint8_t r, uint8_t g, uint8_t b);

// Draws a filled circle at (xc, yc) with the specified radius and RGB color
void PutCircleFilled(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int xc, int yc, int radius, uint8_t r, uint8_t g, uint8_t b);

// Optional framebuffer flush helper
void FlushTheFramebuffer(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo);

#endif // GFX_H
