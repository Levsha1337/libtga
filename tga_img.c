#include <stdlib.h>
#include <stdio.h>

/**
 * Function converts four decimal numbers to one hex in good format for tga.
 * @param  r [red 	channel]
 * @param  g [green channel]
 * @param  b [blue 	channel]
 * @param  a [alpha	channel]
 *
 * @return int [four bytes with info about color]
 */
int tga_color(int r, int g, int b, int a) {
	return a << 24 | r << 16 | g << 8 | b;
}

/**
 * [tga_export description]
 * @param  data [buffer with colors (4 bytes raw rgba)]
 * @param  w    [width]
 * @param  h    [height]
 * @param  n    [file name]
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
