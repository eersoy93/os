#include <stdlib.h>
#include "gfx.h"

void putpixel(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    long location = (x + vinfo->xoffset) * (vinfo->bits_per_pixel / 8) +
                    (y + vinfo->yoffset) * finfo->line_length;
    if (vinfo->bits_per_pixel == 32)
    {
        fbp[location + 0] = b;
        fbp[location + 1] = g;
        fbp[location + 2] = r;
        fbp[location + 3] = 0x00;
    }
    else if (vinfo->bits_per_pixel == 24)
    {
        fbp[location + 0] = b;
        fbp[location + 1] = g;
        fbp[location + 2] = r;
    }
}

void putline(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    while (1)
    {
        putpixel(fbp, vinfo, finfo, x0, y0, r, g, b);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void putbox(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b)
{
    putline(fbp, vinfo, finfo, x0, y0, x1, y0, r, g, b); // top
    putline(fbp, vinfo, finfo, x1, y0, x1, y1, r, g, b); // right
    putline(fbp, vinfo, finfo, x1, y1, x0, y1, r, g, b); // bottom
    putline(fbp, vinfo, finfo, x0, y1, x0, y0, r, g, b); // left
}

void putboxfilled(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b)
{
    if (x0 > x1)
    { 
        int t = x0;
        x0 = x1;
        x1 = t;
    }
    if (y0 > y1)
    {
        int t = y0;
        y0 = y1;
        y1 = t;
    }
    for (int y = y0; y <= y1; ++y)
    {
        putline(fbp, vinfo, finfo, x0, y, x1, y, r, g, b);
    }
}

void putcircle(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int xc, int yc, int radius, uint8_t r, uint8_t g, uint8_t b)
{
    int x = 0, y = radius;
    int d = 3 - 2 * radius;
    while (y >= x)
    {
        putpixel(fbp, vinfo, finfo, xc + x, yc + y, r, g, b);
        putpixel(fbp, vinfo, finfo, xc - x, yc + y, r, g, b);
        putpixel(fbp, vinfo, finfo, xc + x, yc - y, r, g, b);
        putpixel(fbp, vinfo, finfo, xc - x, yc - y, r, g, b);
        putpixel(fbp, vinfo, finfo, xc + y, yc + x, r, g, b);
        putpixel(fbp, vinfo, finfo, xc - y, yc + x, r, g, b);
        putpixel(fbp, vinfo, finfo, xc + y, yc - x, r, g, b);
        putpixel(fbp, vinfo, finfo, xc - y, yc - x, r, g, b);
        x++;
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
    }
}

void putcirclefilled(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int xc, int yc, int radius, uint8_t r, uint8_t g, uint8_t b)
{
    int x = 0, y = radius;
    int d = 3 - 2 * radius;
    while (y >= x)
    {
        putline(fbp, vinfo, finfo, xc - x, yc - y, xc + x, yc - y, r, g, b);
        putline(fbp, vinfo, finfo, xc - y, yc - x, xc + y, yc - x, r, g, b);
        putline(fbp, vinfo, finfo, xc - x, yc + y, xc + x, yc + y, r, g, b);
        putline(fbp, vinfo, finfo, xc - y, yc + x, xc + y, yc + x, r, g, b);
        x++;
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
    }
}