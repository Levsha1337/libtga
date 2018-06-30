
/**
 * github.com/levsha1337/libtga
 *
 * Developed by Levsha1337 (june, 2018)
 *
 * Special thanks to Shinku
 */


#ifndef _tga_img_
#define _tga_img_

extern int tga_export(int *data, int w, int h, const char *n);

extern int tga_color(int r, int g, int b, int a);
// extern int tga_pixel(int **buffer, int x, int y, int w,);

extern int tga_circle(int *buffer, int x, int y, int r, int w, int color);
extern int tga_line(int *buffer, int x0, int y0, int x, int y, int w, int color);


#define tga_pixel(buffer, x, y, w, color) buffer[x + y * w] = color


#endif
