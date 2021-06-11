#include "raster.h"
#include "render.h"

#include "font.c"

typedef struct vertex {
	unsigned int coord[2]; //x, y
	float texcoord[2]; //x, y
	float zcoord; //z
} vertex;

typedef struct edge {
	unsigned int x[2];
	float z[2];
	float texcoord[2][2];
} edge;

vertex vertexbuf[16];
int vertexidx;

unsigned int mode;

unsigned short *frame[] = {(unsigned short *)0x44000000,
	(unsigned short *)0x44044000, (unsigned short *)0x44088000};
unsigned char *vram = (unsigned char *)0x4088000; // Extra vram space
float zbuf[512 * 272];
#define ZBUF(x, y) zbuf[y * 512 + x]
int whichframe = 1;

int _zbuf(int x, int y, float z) {
	if (x > 512 || x < 0 || y > 272 || y < 0)
		return 0;
	if (ZBUF(x, y) >= z)
		return 0;
	ZBUF(x, y) = z;
	return 1;
}


void rast_init() {
	pspDisplaySetMode(0, 480, 272);
	rast_fill_both(0);
	pspDisplaySetFrameBuf((char *)frame[0], 512, 1, 1);
	return;
}

void rast_flip() {
	pspDisplayWaitVblankStart();
	pspDisplaySetFrameBuf((char *)frame[whichframe], 512, 1, 1);
	whichframe++;
	if (whichframe == 3)
		whichframe = 0;
}

void rast_poly_reset() {
	vertexidx = 0;
}

void rast_fill(unsigned short col) {
	unsigned long *px = (unsigned long *)frame[whichframe];
	unsigned long lcol = col << 16 | col;
	int i = 0;
	for (;i < 0x11000;i++)
		px[i] = lcol;
	return;
}

void rast_fill_both(unsigned short col) {
	unsigned long *px = (unsigned long *)frame[0];
	unsigned long lcol = col << 16 | col;
	int i = 0;
	for (;i < 0x22000;i++)
		px[i] = lcol;
	return;
}

void rast_blank_z() {
	int i;
	for (i = 0;i < 0x22000;i++)
		zbuf[i] = -100.0f; //clip at 100, because of small res
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

void rast_drawtext(int x, int y, unsigned short col, char *text) {
	int i, j;
	unsigned short *c;
	while (*text) {
		c = font[(int)*text];
		for (i = 0;i < charh;i++)
				for (j = 0;j < charw;j++)
					if (c[j + i * charw])
						rast_pixel(j + x, i + y, col);
		x += charw;
		text++;
	}
	return;
}

#define SWAP(a, b) {(b) = (a)^(b); (a) ^= (b); (b) ^= (a);}
#define FSWAP(a, b) {float c;c=b;b=a;a=c;}
// from Chip-Nat
#define abs(val) (((val) ^ ((val) >> 31)) + (((val) >> 31)&1))

// {{{ rast_line
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
// }}}

void rast_vertex(int x, int y) {
	vertexbuf[vertexidx].coord[0] = x;
	vertexbuf[vertexidx].coord[1] = y;
	vertexidx++;
}

void rast_coord(int x, int y, float tcx, float tcy, float zc) {
	vertexbuf[vertexidx].coord[0] = x;
	vertexbuf[vertexidx].coord[1] = y;
	vertexbuf[vertexidx].texcoord[0] = tcx;
	vertexbuf[vertexidx].texcoord[1] = tcy;
	vertexbuf[vertexidx].zcoord = zc;
	vertexidx++;
}

void rast_poly_line(unsigned short col) {
	if (vertexidx < 2){ 
		vertexidx = 0;
		return;
	}
	rast_line(vertexbuf[0].coord[0],vertexbuf[0].coord[1],
			vertexbuf[vertexidx - 1].coord[0],
			vertexbuf[vertexidx - 1].coord[1], col);
	if (vertexidx == 2) {
		vertexidx = 0;
		return;
	}

	int i;
	for (i = 1;i < vertexidx;i++)
		rast_line(vertexbuf[i-1].coord[0],vertexbuf[i-1].coord[1],
			vertexbuf[i].coord[0],vertexbuf[i].coord[1], col);
	vertexidx = 0;
	return;
}

edge xbuf[272];

inline void _setbuf(int x, int y, float zc, float tc[]) {
	if (x > 480)
		x = 480;
	else if (x < 0)
		x = 0;
	if (y > 271 || y < 0)
		return;
	
	if (x < xbuf[y].x[0]) {
		xbuf[y].x[0] = x;
		xbuf[y].z[0] = zc;
		xbuf[y].texcoord[0][0] = tc[0];
		xbuf[y].texcoord[0][1] = tc[1];
	}
	if (x > xbuf[y].x[1]) {
		xbuf[y].x[1] = x;
		xbuf[y].z[1] = zc;
		xbuf[y].texcoord[1][0] = tc[0];
		xbuf[y].texcoord[1][1] = tc[1];
	}
	return;
}

// {{{ rast_poly_fill
//edge buffered poly fill
//Only supports convex polys
void rast_poly_fill(unsigned short col) {
		int i;
	for (i = 0;i < 272;i++) {
		xbuf[i].x[0] = 0xFFFFFFFF; // (2 ^ 32) - 1
		xbuf[i].x[1] = 0;
	}


	if (vertexidx < 2) {
		vertexidx = 0;
		return;
	}
		
	if (vertexidx == 2) {
		rast_line(vertexbuf[0].coord[0],vertexbuf[0].coord[1],
				vertexbuf[1].coord[0],
				vertexbuf[1].coord[1], col);
		vertexidx = 0;
		return;
	}

	//complete the loop
	rast_coord(vertexbuf[0].coord[0], vertexbuf[0].coord[1],
			0, 0, vertexbuf[0].zcoord);

	int sx, sy, dx, dy, x, y;
	float len;
	float z, zs;
	float sz, dz; //sz = source z, zs = z step
	float tc[] = {0,0}; //not used
	for (i = 1;i < vertexidx;i++) {
		sx = vertexbuf[i-1].coord[0];
		sy = vertexbuf[i-1].coord[1];
		sz = vertexbuf[i-1].zcoord;
		dx = vertexbuf[i].coord[0];
		dy = vertexbuf[i].coord[1];
		dz = vertexbuf[i].zcoord;

	//straight lines
	if (sx == dx) {
		if (sy > dy){
			SWAP(sy, dy);
			FSWAP(sz, dz);
		}
		len = dy - sy;
		if (len == 0)
			len = 0.0000001f;
		zs = (dz - sz) / len;
		z = sz;
		for (y = sy; y < dy;y++) {
			_setbuf(sx, y, z, tc);
			z += zs;
		}
	} else if (sy == dy) {
		if (sx > dx){
			SWAP(sx, dx);
			FSWAP(sz, dz);
		}
		_setbuf(sx, sy, sz, tc);
		_setbuf(dx, sy, dz, tc);
	} else {

	//drop 1 indent level
	//bresenham's (complete) algo
	int steep = abs(dy - sy) > abs(dx - sx);
	if (steep) {
		SWAP(sx, sy);
		SWAP(dx, dy);
		FSWAP(sz, dz);
	}

	int deltx = abs(dx - sx);
	int delty = abs(dy - sy);
	int error = 0;

	int delterr = delty;
	x = sx;
	y = sy;

	int xs = (sx < dx) ? 1 : -1;
	int ys = (sy < dy) ? 1 : -1;

	zs = (dz - sz) / deltx;
	z = sz;

	if (steep) _setbuf(y, x, z, tc);
	else _setbuf(x, y, z, tc);

	while (x != dx) {
		x += xs;
		z += zs;

		error += delterr;
		if ((error << 1) >= deltx) {
			y += ys;
			error -= deltx;
		}
		if (steep) _setbuf(y, x, z, tc);
		else _setbuf(x, y, z, tc);

	} //drawing loop
	
	} //bresenham's algo
	} //for lines
	
	//now, go through the xbuf and draw stuff
	for (i = 0;i < 272;i++) {
		if (xbuf[i].x[0] < xbuf[i].x[1]) {
			zs = (xbuf[i].z[1] - xbuf[i].z[0]) /
				(xbuf[i].x[1] - xbuf[i].x[0]);
			z = xbuf[i].z[0];
			
			for (x = xbuf[i].x[0];x < xbuf[i].x[1];x++){
				z += zs;
				if (_zbuf(x, i, z))
					rast_pixel(x, i, col);
			}
		}
	}
	
	vertexidx = 0;

	return;
}
// }}}

inline void _texture(int x, int y, float tx, float ty) {
	rast_pixel(x, y, rend_texture_point(tx, ty));
	return;
}

// {{{ rast_poly_textured
//edge buffered poly fill
//Only supports convex polys
void rast_poly_textured() {
	int i;
	for (i = 0;i < 272;i++) {
		xbuf[i].x[0] = 0xFFFFFFFF; // (2 ^ 32) - 1
		xbuf[i].x[1] = 0;
	}

	if (vertexidx < 2) {
		vertexidx = 0;
		return;
	}
		
	if (vertexidx == 2) {
		vertexidx = 0;
		return;
	}

	//complete the loop
	rast_coord(vertexbuf[0].coord[0], vertexbuf[0].coord[1],
			vertexbuf[0].texcoord[0], vertexbuf[0].texcoord[1],
			vertexbuf[0].zcoord);

	int sx, sy, dx, dy, x, y;
	float len;
	float z, zs, tc[2], tcs[2];
	float sz, dz, stc[2], dtc[2]; //sz = source z, zs = z step
	for (i = 1;i < vertexidx;i++) {
		sx = vertexbuf[i-1].coord[0];
		sy = vertexbuf[i-1].coord[1];
		sz = vertexbuf[i-1].zcoord;
		stc[0] = vertexbuf[i-1].texcoord[0];
		stc[1] = vertexbuf[i-1].texcoord[1];
		dx = vertexbuf[i].coord[0];
		dy = vertexbuf[i].coord[1];
		dz = vertexbuf[i].zcoord;
		dtc[0] = vertexbuf[i].texcoord[0];
		dtc[1] = vertexbuf[i].texcoord[1];

	//straight lines
	if (sx == dx) {
		if (sy > dy){
			SWAP(sy, dy);
			FSWAP(dz, sz);
			FSWAP(stc[0], dtc[0]);
			FSWAP(stc[1], dtc[1]);
		}
		len = dy - sy;
		if (len == 0)
			len = 0.0000001f;
		zs = (dz - sz) / len;
		z = sz;
		
		tcs[0] = (dtc[0] - stc[0]) / len;
		tcs[1] = (dtc[1] - stc[1]) / len;
		tc[0] = stc[0];
		tc[1] = stc[1];
		for (y = sy; y < dy;y++) {
			_setbuf(sx, y, z, tc);
			z += zs;
			tc[0] += tcs[0];
			tc[1] += tcs[1];
		}
	} else if (sy == dy) {
		//special case, only need to call setbuf twice.
		if (sx > dx) {
			SWAP(sx, dx);
			FSWAP(dz, sz);
			FSWAP(stc[0], dtc[0]);
			FSWAP(stc[1], dtc[1]);
		}
		_setbuf(sx, sy, sz, stc);
		_setbuf(dx, sy, dz, dtc);

	} else {

	//drop 1 indent level
	//bresenham's (complete) algo
	int steep = abs(dy - sy) > abs(dx - sx);
	if (steep) {
		SWAP(sx, sy);
		SWAP(dx, dy);
		FSWAP(dz, sz);
		FSWAP(stc[0], dtc[0]);
		FSWAP(stc[1], dtc[1]);
	}

	int deltx = abs(dx - sx);
	int delty = abs(dy - sy);
	int error = 0;

	int delterr = delty;
	x = sx;
	y = sy;

	int xs = (sx < dx) ? 1 : -1;
	int ys = (sy < dy) ? 1 : -1;

	zs = (dz - sz) / deltx;
	tcs[0] = (dtc[0] - stc[0]) / deltx;
	tcs[1] = (dtc[1] - stc[1]) / deltx;
	z = sz;
	tc[0] = stc[0];
	tc[1] = stc[1];


	if (steep) _setbuf(y, x, z, tc);
	else _setbuf(x, y, z, tc);

	while (x != dx) {
		x += xs;
		z += zs;
		tc[0] += tcs[0];
		tc[1] += tcs[1];

		error += delterr;
		if ((error << 1) >= deltx) {
			y += ys;
			error -= deltx;
		}
		if (steep) _setbuf(y, x, z, tc);
		else _setbuf(x, y, z, tc);

	} //drawing loop
	
	} //bresenham's algo
	} //for lines
	
	//now, go through the xbuf and draw stuff
	for (i = 0;i < 272;i++) {
		if (xbuf[i].x[0] < xbuf[i].x[1]) {
			zs = (xbuf[i].z[1] - xbuf[i].z[0]) /
				(xbuf[i].x[1] - xbuf[i].x[0]);
			z = xbuf[i].z[0];
			
			tcs[0] = (xbuf[i].texcoord[1][0] - 
					xbuf[i].texcoord[0][0]) / 
					(xbuf[i].x[1] - xbuf[i].x[0]);
			tcs[1] = (xbuf[i].texcoord[1][1] - 
					xbuf[i].texcoord[0][1]) / 
					(xbuf[i].x[1] - xbuf[i].x[0]);
			tc[0] = xbuf[i].texcoord[0][0];
			tc[1] = xbuf[i].texcoord[0][1];

			for (x = xbuf[i].x[0];x < xbuf[i].x[1];x++){
				z += zs;
				tc[0] += tcs[0];
				tc[1] += tcs[1];
				if (_zbuf(x, i, z))
					_texture(x, i, tc[0], tc[1]);
			}
		}
	}
	
	vertexidx = 0;

	return;
}
// }}}

