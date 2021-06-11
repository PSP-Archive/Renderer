#include "raster.h"

int vertexbuf[2][16];
int vertexidx;

unsigned short *frame[] = {(unsigned short *)0x04000000,
	(unsigned short *)0x04044000};
unsigned char *vram = (unsigned char *)0x4088000; // Extra vram space
int whichframe = 1;

void rast_init() {
	rast_fill_both(0);
	pspDisplaySetMode(0, 480, 272);
	pspDisplaySetFrameBuf((char *)frame[0], 512, 1, 1);
	return;
}

void rast_flip() {
	pspDisplayWaitVblankStart();
	whichframe ^= 1;
	pspDisplaySetFrameBuf((char *)frame[whichframe], 512, 1, 1);
}

void rast_fill(unsigned short col) {
	unsigned long *px = (unsigned long *)frame[whichframe];
	unsigned long lcol = col << 16 | col;
	int i = 0;
	for (i;i < 0x11000;i++)
		px[i] = lcol;
	return;
}

void rast_fill_both(unsigned short col) {
	unsigned long *px = (unsigned long *)frame[0];
	unsigned long lcol = col << 16 | col;
	int i = 0;
	for (i;i < 0x22000;i++)
		px[i] = lcol;
	return;
}

inline void rast_pixel(int x, int y, unsigned short col) {
	if (x > 480 || x < 0 || y > 272 || y < 0)
		return;
	PIXEL(whichframe, x, y, col);
}

//dest, then src
void rast_blit(int dx, int dy, unsigned short *src, int sw, int sh) {
	int i, j;
	for (i = dy;i < (dy + sh);i++) {
		for (j = dx;j < (dx + sw);j++) {
			PIXEL(whichframe, j, i, *src++);
		}
	}
	return;
}

#define SWAP(a, b) {(b) = (a)^(b); (a) ^= (b); (b) ^= (a);}
// from Chip-Nat
#define abs(val) (val ^ (val >> 31)) + ((val >> 31)&1)

void rast_line(int sx, int sy, int dx, int dy, unsigned short col) {
	int x, y;
	if (sx == dx && sy == dy) {
		rast_pixel(sx, sy, col);
		return;
	}
	if (sx == dx) {
		if (sy > dy) SWAP(sy, dy);
		for (y = sy; y < dy;y++)
			rast_pixel(sx, y, col);
		return;
	} else if (sy == dy) {
		if (sx > dx) SWAP(sx, dx);
		for (x = sx;x < dx;x++)
			rast_pixel(x, sy, col);
		return;
	}

	
	//bresenham's (complete) algo
	int steep = abs(dy - sy) > abs(dx - sx);
	if (steep) {
		SWAP(sx, sy);
		SWAP(dx, dy);
	}

	int deltx = abs(dx - sx);
	int delty = abs(dy - sy);
	int error = 0;

	int delterr = delty;
	x = sx;
	y = sy;

	int xs = (sx < dx) ? 1 : -1;
	int ys = (sy < dy) ? 1 : -1;

	if (steep) rast_pixel(y, x, col);
	else rast_pixel (x, y, col);

	while (x != dx) {
		x += xs;
		error += delterr;
		if ((error << 1) >= deltx) {
			y += ys;
			error -= deltx;
		}
		if (steep) rast_pixel(y, x, col);
		else rast_pixel(x, y, col);
	}
	
	return;
}

void rast_vertex(int x, int y) {
	//safety and clipping
	if (x > 480)
		x = 480;
	else if (x < 0)
		x = 0;
	if (y > 272)
		y = 272;
	else if (y < 0)
		y = 0;
	vertexbuf[vertexidx][0] = x;
	vertexbuf[vertexidx][1] = y;
	vertexidx++;
}

void rast_poly_line(unsigned short col) {
	if (vertexidx < 2){ 
		vertexidx = 0;
		return;
	}
	rast_line(vertexbuf[0][0],vertexbuf[0][1], vertexbuf[vertexidx - 1][0],
			vertexbuf[vertexidx - 1][1], col);
	if (vertexidx == 2) {
		vertexidx = 0;
		return;
	}

	int i;
	for (i = 1;i < vertexidx;i++)
		rast_line(vertexbuf[i-1][0],vertexbuf[i-1][1],
			vertexbuf[i][0],vertexbuf[i][1], col);
	vertexidx = 0;
	return;
}

inline void _setbuf(int x, unsigned int buf[]) {
	if (x > 480 || x < 0)
		return;
	if (x < buf[0])
		buf[0] = x;
	if (x > buf[1])
		buf[1] = x;
	return;
}

//edge buffered poly fill
//Only supports convex polys
void rast_poly_fill(unsigned short col) {
	unsigned int xbuf[272][2]; // 2 x values for each y val
	int i;
	for (i = 0;i < 272;i++) {
		xbuf[i][0] = 0xFFFFFFFF; // (2 ^ 32) - 1
		xbuf[i][1] = 0;
	}

	if (vertexidx < 2) {
		vertexidx = 0;
		return;
	}
		
	if (vertexidx == 2) {
		rast_line(vertexbuf[0][0],vertexbuf[0][1],
				vertexbuf[1][0],
				vertexbuf[1][1], col);
		vertexidx = 0;
		return;
	}

	//complete the loop
	rast_vertex(vertexbuf[0][0], vertexbuf[0][1]);

	int sx, sy, dx, dy, x, y;
	for (i = 1;i < vertexidx;i++) {
		sx = vertexbuf[i-1][0];
		sy = vertexbuf[i-1][1];
		dx = vertexbuf[i][0];
		dy = vertexbuf[i][1];
	
	//straight lines
	if (sx == dx) {
		if (sy > dy) SWAP(sy, dy);
		for (y = sy; y < dy;y++)
			_setbuf(sx, xbuf[y]);
	} else if (sy == dy) {
		if (sx > dx) SWAP(sx, dx);
		for (x = sx;x < dx;x++)
			_setbuf(x, xbuf[sy]);
	} else {

	
	//bresenham's (complete) algo
	int steep = abs(dy - sy) > abs(dx - sx);
	if (steep) {
		SWAP(sx, sy);
		SWAP(dx, dy);
	}

	int deltx = abs(dx - sx);
	int delty = abs(dy - sy);
	int error = 0;

	int delterr = delty;
	x = sx;
	y = sy;

	int xs = (sx < dx) ? 1 : -1;
	int ys = (sy < dy) ? 1 : -1;

	if (steep) _setbuf(y, xbuf[x]);
	else _setbuf(x, xbuf[y]);

	while (x != dx) {
		x += xs;
		error += delterr;
		if ((error << 1) >= deltx) {
			y += ys;
			error -= deltx;
		}
		if (steep) _setbuf(y, xbuf[x]);
		else _setbuf(x, xbuf[y]);

	}
	
	}
	}

	//now, go through the xbuf and draw stuff
	for (i = 0;i < 272;i++) {
		if (xbuf[i][0] < xbuf[i][1]) {
			for (x = xbuf[i][0];x < xbuf[i][1];x++)
				rast_pixel(x, i, col);
		}
	}
	
	vertexidx = 0;

	return;
}

