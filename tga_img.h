
/**
 * github.com/levsha1337/lib_tga
 *
 * Developed by Levsha1337 (june, 2018)
 *
 * Special thanks to Shinku
 */


#ifndef _tga_img_
#define _tga_img_

extern int tga_export(int *data, int w, int h, const char *n);
extern int tga_color(int r, int g, int b, int a);

#define tga_pixel(buffer, x, y, w, color) buffer[x + y * w] = color

#endif
