#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

// FreeType for TTF rendering
#include <ft2build.h>
#include FT_FREETYPE_H

#include "gfx.h"

void PutPixel(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int x, int y, uint8_t r, uint8_t g, uint8_t b)
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

void PutLine(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    while (1)
    {
        PutPixel(fbp, vinfo, finfo, x0, y0, r, g, b);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void PutBox(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b)
{
    PutLine(fbp, vinfo, finfo, x0, y0, x1, y0, r, g, b); // top
    PutLine(fbp, vinfo, finfo, x1, y0, x1, y1, r, g, b); // right
    PutLine(fbp, vinfo, finfo, x1, y1, x0, y1, r, g, b); // bottom
    PutLine(fbp, vinfo, finfo, x0, y1, x0, y0, r, g, b); // left
}

void PutBoxFilled(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b)
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
        PutLine(fbp, vinfo, finfo, x0, y, x1, y, r, g, b);
    }
}

void PutCircle(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int xc, int yc, int radius, uint8_t r, uint8_t g, uint8_t b)
{
    int x = 0, y = radius;
    int d = 3 - 2 * radius;
    while (y >= x)
    {
        PutPixel(fbp, vinfo, finfo, xc + x, yc + y, r, g, b);
        PutPixel(fbp, vinfo, finfo, xc - x, yc + y, r, g, b);
        PutPixel(fbp, vinfo, finfo, xc + x, yc - y, r, g, b);
        PutPixel(fbp, vinfo, finfo, xc - x, yc - y, r, g, b);
        PutPixel(fbp, vinfo, finfo, xc + y, yc + x, r, g, b);
        PutPixel(fbp, vinfo, finfo, xc - y, yc + x, r, g, b);
        PutPixel(fbp, vinfo, finfo, xc + y, yc - x, r, g, b);
        PutPixel(fbp, vinfo, finfo, xc - y, yc - x, r, g, b);
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

void PutCircleFilled(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo, int xc, int yc, int radius, uint8_t r, uint8_t g, uint8_t b)
{
    int x = 0, y = radius;
    int d = 3 - 2 * radius;
    while (y >= x)
    {
        PutLine(fbp, vinfo, finfo, xc - x, yc - y, xc + x, yc - y, r, g, b);
        PutLine(fbp, vinfo, finfo, xc - y, yc - x, xc + y, yc - x, r, g, b);
        PutLine(fbp, vinfo, finfo, xc - x, yc + y, xc + x, yc + y, r, g, b);
        PutLine(fbp, vinfo, finfo, xc - y, yc + x, xc + y, yc + x, r, g, b);
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

void FlushTheFramebuffer(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo)
{
    // This function is optional and may not be needed for all framebuffer drivers.
    // It can be used to ensure that the framebuffer is updated on the display.
    // Many drivers automatically update the display from the mmap'd buffer.
    msync(fbp, (size_t)vinfo->yres_virtual * finfo->line_length, MS_SYNC);
}

// Alpha blend a colored pixel over the framebuffer at (px, py)
static inline void PutPixelBlend(uint8_t *fbp, const struct fb_var_screeninfo *vinfo, const struct fb_fix_screeninfo *finfo,
                                int px, int py, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    if (a == 0) return;
    if (px < 0 || py < 0 || px >= (int)vinfo->xres || py >= (int)vinfo->yres) return;
    long location = (px + vinfo->xoffset) * (vinfo->bits_per_pixel / 8) +
                    (py + vinfo->yoffset) * finfo->line_length;
    if (vinfo->bits_per_pixel == 32)
    {
        uint8_t db = fbp[location + 0];
        uint8_t dg = fbp[location + 1];
        uint8_t dr = fbp[location + 2];
        // uint8_t da = fbp[location + 3]; // unused
        // out = src*a + dst*(1-a)
        uint8_t or_ = (uint8_t)((r * a + dr * (255 - a)) / 255);
        uint8_t og_ = (uint8_t)((g * a + dg * (255 - a)) / 255);
        uint8_t ob_ = (uint8_t)((b * a + db * (255 - a)) / 255);
        fbp[location + 0] = ob_;
        fbp[location + 1] = og_;
        fbp[location + 2] = or_;
        fbp[location + 3] = 0x00;
    }
    else if (vinfo->bits_per_pixel == 24)
    {
        uint8_t db = fbp[location + 0];
        uint8_t dg = fbp[location + 1];
        uint8_t dr = fbp[location + 2];
        uint8_t or_ = (uint8_t)((r * a + dr * (255 - a)) / 255);
        uint8_t og_ = (uint8_t)((g * a + dg * (255 - a)) / 255);
        uint8_t ob_ = (uint8_t)((b * a + db * (255 - a)) / 255);
        fbp[location + 0] = ob_;
        fbp[location + 1] = og_;
        fbp[location + 2] = or_;
    }
}

void PutText(uint8_t *fbp, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo,
             int x, int y, const char *text, const char *font_path, uint8_t r, uint8_t g, uint8_t b)
{
    if (!fbp || !vinfo || !finfo || !text || !font_path) return;
    if (!(vinfo->bits_per_pixel == 24 || vinfo->bits_per_pixel == 32)) return;

    FT_Library ft;
    if (FT_Init_FreeType(&ft) != 0) return;

    FT_Face face;
    if (FT_New_Face(ft, font_path, 0, &face) != 0)
    {
        FT_Done_FreeType(ft);
        return;
    }

    // Choose a reasonable default pixel height
    const int pixel_height = 24;
    FT_Set_Pixel_Sizes(face, 0, pixel_height);

    // Baseline position: interpret (x,y) as top-left baseline origin
    int pen_x = x;
    int pen_y = y + (int)(face->size->metrics.ascender >> 6); // ascender in pixels

    for (const unsigned char *p = (const unsigned char *)text; *p; ++p)
    {
        unsigned int ch = *p;
        if (ch == '\n')
        {
            pen_x = x;
            pen_y += (int)(face->size->metrics.height >> 6);
            continue;
        }

        if (FT_Load_Char(face, ch, FT_LOAD_RENDER) != 0)
            continue;

        FT_GlyphSlot gslot = face->glyph;
        FT_Bitmap *bmp = &gslot->bitmap;
        int gx = pen_x + gslot->bitmap_left;
        int gy = pen_y - gslot->bitmap_top;

        // Draw bitmap into framebuffer with alpha blending
        for (int row = 0; row < (int)bmp->rows; ++row)
        {
            for (int col = 0; col < (int)bmp->width; ++col)
            {
                uint8_t a = bmp->buffer[row * bmp->pitch + col];
                if (a)
                {
                    PutPixelBlend(fbp, vinfo, finfo, gx + col, gy + row, r, g, b, a);
                }
            }
        }

        pen_x += (int)(gslot->advance.x >> 6);
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}
