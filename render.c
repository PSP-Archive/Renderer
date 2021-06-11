#include "matrix.h"
#include "raster.h"

#define ID_MATRIX {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}
#define FOV 100

float world[16] = ID_MATRIX; //ident matrix
unsigned short col = 0xffff;
float polybuf[16][3]; //to store each poly
int polyindex = 0;    //current index into the buffer
int type = 0;

void rend_reset() {
	polyindex = 0;
	world[0] = 1;
	world[1] = 0;
	world[2] = 0;
	world[3] = 0;
	
	world[4] = 0;
	world[5] = 1;
	world[6] = 0;
	world[7] = 0;
	
	world[8] = 0;
	world[9] = 0;
	world[10] = 1;
	world[11] = 0;
	
	world[12] = 0;
	world[13] = 0;
	world[14] = 0;
	world[15] = 1;
	return;
}

void rend_translate(float x, float y, float z) {
	//matrix_translate(world, x, y, z);
	world[12] += x;
	world[13] += y;
	world[14] += z;
}

void rend_rotate(float x, float y, float z, float angle) {
	float matrix[16] = ID_MATRIX, tmp[16] = ID_MATRIX;
	float vert[3] = {x, y, z};
	int i; //goddamn, not having malloc() sucks.

	matrix_rotate_around_axis(matrix, vert, angle);
	matrix_multiplyM(tmp, matrix, world);
	for (i = 0;i < 16;i++)
		world[i] = tmp[i];
	return;
}

//y and z negated to match OGL
inline int rastx(float x, float z) {
	if (z >= 0)
		return 0; //will be clipped out of existence
	return (int)(x * FOV / -z) + 240;
}
inline int rasty(float y, float z) {
	if (z >= 0)
		return 0;
	return (int)(-y * FOV / -z) + 136;
}

inline void _draw(float x, float y, float z) {
	rast_pixel(rastx(x, z), rasty(y, z), col); 
}

void rend_point(float x, float y, float z) {
	float vert[3] = {x, y, z};
	matrix_multiplyV(vert, world, vert);
	_draw(vert[0], vert[1], vert[2]);
}

void rend_colour(unsigned short colour) {
	col = colour;
	return;
}

void rend_line (float x1, float y1, float z1, float x2, float y2, float z2) {

	float vert1[3] = {x1, y1, z1}, vert2[3] = {x2, y2,z2};
	matrix_multiplyV(vert1, world, vert1);
	matrix_multiplyV(vert2, world, vert2);
	
	rast_line(rastx(vert1[0], vert1[2]), rasty(vert1[1], vert1[2]),
		  rastx(vert2[0], vert2[2]), rasty(vert2[1], vert2[2]),
			col);
	return;
}

void rend_vertex(float x, float y, float z) {
	float vert[3] = {x, y, z};
	matrix_multiplyV(vert, world, vert);

	polybuf[polyindex][0] = vert[0];
	polybuf[polyindex][1] = vert[1];
	polybuf[polyindex][2] = vert[2];
	polyindex++;
	return;
}


void _poly(int mode) {
	int i;
	if (polyindex < 2) {
		polyindex = 0;
		return; //need at least a line...
	}

	for (i = 0;i < polyindex;i++) {
		rast_vertex(rastx(polybuf[i][0], polybuf[i][2]),
				rasty(polybuf[i][1], polybuf[i][2]));
	}
	switch(mode) {
		case 0:
			rast_poly_line(col);
			break;
		case 1:
			rast_poly_fill(col);
			break;
	}
	polyindex = 0;

	return;
}

void rend_poly_fill() {
	_poly(1);
	return;
}

void rend_poly_line() {
	_poly(0);
	return;
}
