#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define tga_pixel(buffer, x, y, w, color) buffer[x + y * w] = color

/**
 * Function converts four decimal numbers to one hex in good format for tga.
 * @param  r [red   channel]
 * @param  g [green channel]
 * @param  b [blue  channel]
 * @param  a [alpha channel]
 *
 * @return int [four bytes with info about pixel]
 */
int tga_color(int r, int g, int b, int a) {
	return a << 24 | r << 16 | g << 8 | b;
}

/**
 * Function draws 1px line from [x0, y0] to [x, y].
 * @param  buffer [buffer with pixels info (4 bytes raw rgba from tga_color)]
 * @param  x0     [x start pos]
 * @param  y0     [y start pos]
 * @param  x      [x final pos]
 * @param  y      [y final pos]
 * @param  w      [width of image]
 * @param  color  [raw rgba color]
 * 
 * @return int    [always 0, on this step]
 */
int tga_line(int *buffer, int x0, int y0, int x, int y, int w, int color) {

	/* from rosettacode.org */
	int dx = abs(x - x0), sx = x0 < x ? 1 : -1;
	int dy = abs(y - y0), sy = y0 < y ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;

	for (;;) {
		tga_pixel(buffer, x0, y0, w, color);

		if (x0 == x && y0 == y) break;

		e2 = err;
		if (e2 > -dx) { err -= dy; x0 += sx; }
		if (e2 <  dy) { err += dx; y0 += sy; }
	}

	return 0;
}

/**
 * Function draws circle on [x, y] with radius r.
 * @param  buffer [buffer with pixels info (4 bytes raw rgba from tga_color)]
 * @param  x      [center x]
 * @param  y      [center y]
 * @param  r      [raduis]
 * @param  w      [width of image]
 * @param  color  [raw rgba color]
 * @return int    [always 0, on this step]
 */
int tga_circle(int *buffer, int x, int y, int r, int w, int color) {

	for (int j = y-r; j <= y+r; j++) {
		int ay = y - j;

		for (int i = x-r; i <= x+r; i++) {
			int ax = x - i;

			float hyp2 = ay*ay + ax*ax;
			float r2 = r*r;

			if (hyp2 <= r2) tga_pixel(buffer, i, j, w, color);
			else continue;
		}
	}

	return 0;
}

/**
 * Function write data to Targa (.tga) file
 * @param  data [buffer with pixels info (4 bytes raw rgba from tga_color)]
 * @param  w    [width]
 * @param  h    [height]
 * @param  n    [out file name]
 *
 * @return int  [Exit code (0 - good, 1...4 - bad)]
 */
int tga_export(int *data, int w, int h, const char *n) {
	const int *p = data;     // take data about our image in raw 32bit format
	if (p == NULL) return 1; // no data, image handle points to invalid image

	FILE *f = fopen(n, "wb+");
	if (f == NULL) return 2; // can not open file

    // prepare header to write
	const unsigned char length = 0;
	const char cmap_type = 0;
	const char type = 2; // uncompressed rgb type
	const unsigned short cmap_offset = 0;
	const unsigned short cmap_length = 0;
	const char cmap_depth = 0;
	const short xorigin = 0; // x left-top pos
	const short yorigin = 0; // y left-top pos
	const unsigned short width = w; // get image width and height
	const unsigned short height = h;
	const char bits = 32; // size of data about one pixel (rgba)
	const char flags = 8 | (0 << 5); // 8 bit alpha + vertical flip

	const int px = (int) width * (int) height; // total pixels

	size_t c = 0; // count of written bytes

	// write header
	c += fwrite(&length, sizeof(unsigned char), 1, f);
	c += fwrite(&cmap_type, sizeof(char), 1, f);
	c += fwrite(&type, sizeof(char), 1, f);
	c += fwrite(&cmap_offset, sizeof(unsigned short), 1, f);
	c += fwrite(&cmap_length, sizeof(unsigned short), 1, f);
	c += fwrite(&cmap_depth, sizeof(char), 1, f);
	c += fwrite(&xorigin, sizeof(short), 1, f);
	c += fwrite(&yorigin, sizeof(short), 1, f);
	c += fwrite(&width, sizeof(unsigned short), 1, f);
	c += fwrite(&height, sizeof(unsigned short), 1, f);
	c += fwrite(&bits, sizeof(char), 1, f);
	c += fwrite(&flags, sizeof(char), 1, f);

	int y; // vertical flip (or we could just set FLAGS to 8 | (1 << 5))
	for (y = height - 1; y >= 0; y--) {
		c += fwrite(p + y * width, sizeof(int), width, f);
	}

	if (fclose(f) == EOF) return 3; // make sure to close file
	if (c != px + 12) return 4;     // 12 is header length, check that all bytes are written correctly

	return 0; // all good, image saved, return good exit code
}
