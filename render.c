#include "matrix.h"
#include "raster.h"

#define ID_MATRIX {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}
#define FOV 120

float mxstack[16][16] = {ID_MATRIX};
float *world = mxstack[0];
int matrix = 0;
unsigned short col = 0xffff;
float polybuf[16][5]; //to store each poly & texcoord
int polyindex = 0;    //current index into the buffer
int type = 0;
unsigned short *texture;
float texturew, textureh;

void rend_reset() {
	polyindex = 0;
	world = mxstack[0];
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

int rend_pushmatrix() {
	int i;
	if (matrix == 15)
		return -1; //no more room!
	for (i = 0;i < 16;i++)
		mxstack[matrix + 1][i] = mxstack[matrix][i];
	world = mxstack[++matrix];
	return 0;
}

int rend_popmatrix() {
	if (matrix == 0)
		return -1; //base of the stack
	world = mxstack[--matrix];
	return 0;
}

void rend_rotate(float x, float y, float z, float angle) {
	float matrix[16] = ID_MATRIX, tmp[16] = ID_MATRIX;
	float vert[3] = {x, y, z};
	int i; //goddamn, not having malloc() sucks.

	matrix_rotate_around_axis(matrix, vert, angle);
	matrix_multiplyM(tmp, world, matrix);
	for (i = 0;i < 16;i++)
		world[i] = tmp[i];
	return;
}

//why clip at 1? If you clip at less than 1, the x and y can get blown
//insanely big. Then the renderer dies.
inline int rastx(float x, float z) {
	if (z >= -1)
		z = -1;
	if (z < -100)
		z = -100;
	return (int)(x * FOV / -z) + 240;
}
inline int rasty(float y, float z) {
	if (z >= -1)
		z = -1;
	else if (z < -100)
		z = -100;
	return (int)(-y * FOV / -z) + 136;
}

void rend_colour(unsigned short colour) {
	col = colour;
	return;
}

void rend_texture(unsigned short *tex, int w, int h) {
	texture = tex;
	texturew = w;
	textureh = h - 1;
	return;
}

inline unsigned short rend_texture_point(float tcx, float tcy) {
	//clip coords
	if (tcx > 1.0f)
		tcx = 1.0f;
	if (tcy > 1.0f)
		tcy = 1.0f;
	int x = (int)(texturew * tcx);
	int y = (int)(textureh * tcy);
	return texture[x + y * (int)texturew];
	
}

inline void _draw(float x, float y, float z) {
	rast_pixel(rastx(x, z), rasty(y, z), col); 
}

void rend_point(float x, float y, float z) {
	float vert[3] = {x, y, z};
	matrix_multiplyV(vert, world, vert);
	_draw(vert[0], vert[1], vert[2]);
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

void rend_vertex_tc(float x, float y, float z, float tx, float ty) {
	float vert[3] = {x, y, z};
	matrix_multiplyV(vert, world, vert);

	polybuf[polyindex][0] = vert[0];
	polybuf[polyindex][1] = vert[1];
	polybuf[polyindex][2] = vert[2];
	polybuf[polyindex][3] = tx;
	polybuf[polyindex][4] = ty;

	polyindex++;
	return;
}

void rend_poly_line() {
	int i;
	if (polyindex < 2) {
		polyindex = 0;
		return; //need at least a line...
	}

	for (i = 0;i < polyindex;i++) {
		rast_vertex(rastx(polybuf[i][0], polybuf[i][2]),
				rasty(polybuf[i][1], polybuf[i][2]));
	}
	rast_poly_line(col);
	polyindex = 0;

	return;
}

//backface cull

int _poly_cull() {
	float view[4] = {polybuf[0][0], polybuf[0][1], polybuf[0][2]};
	float normal[4];

	matrix_normalizeV(view);

	matrix_normal(normal, polybuf[0], polybuf[1], polybuf[2]);
	normal[3] = 0;
	if (matrix_dotproduct(normal, view) < 0)
		return 1;
	return 0;
}

void rend_poly_fill() {
	int i;
	if (polyindex < 3) {
		polyindex = 0;
		return;
	}

	if (!_poly_cull()){
		polyindex = 0;
		return;
	}

	for (i = 0;i < polyindex;i++) {
		rast_coord(rastx(polybuf[i][0], polybuf[i][2]),
				rasty(polybuf[i][1], polybuf[i][2]),
				0,0,polybuf[i][2]);
	}
	rast_poly_fill(col);
	polyindex = 0;

	return;
}

void rend_poly_textured() {
	int i;
	if (polyindex < 3) {
		polyindex = 0;
		return; //need at least a line...
	}

	if (!_poly_cull()) {
		polyindex = 0;
		return;
	}

	for (i = 0;i < polyindex;i++) {
		rast_coord(rastx(polybuf[i][0], polybuf[i][2]),
				rasty(polybuf[i][1], polybuf[i][2]),
				polybuf[i][3],polybuf[i][4],polybuf[i][2]);
	}
	rast_poly_textured();
	polyindex = 0;

	return;
}


