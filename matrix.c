#include "trig.h"
#include <stdlib.h>
#include <string.h>

//we ALWAYS work with 4x4 matrices
#define ID_MATRIX {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}

void matrix_create_identity(float *m) {
	/*
	float *m = malloc(sizeof(float) * 16);
	memcpy(m, matrix_ID, sizeof(float) * 16);
	*/
	m[0] = m[5] = m[10] = m[15] = 1;
	return;
}

void matrix_translate(float *m, float x, float y, float z) {
	//right row of the matrix is translation
	m[12] += x;
	m[13] += y;
	m[14] += z;
	return;
}

void matrix_add(float *dst, float *src) {
	int i;
	for (i = 0;i < 16;i++)
		dst[i] += src[i];
	return;
}

void matrix_multiplyS(float *m, float ce) {//ce =coefficient, S = scalar
	int i;
	for (i = 0;i < 16;i++)
		m[i] *= ce;
	return;
}

//m1 and m2 are arrays of *4* elements
inline float matrix_dotproduct(float *m1, float *m2) {
	return  (m1[0] * m2[0] + 
		m1[1] * m2[1] +
		m1[2] * m2[2] +
		m1[3] * m2[3]);
}

//cross product between vectors
//not really a matrix function, but close
void matrix_crossproduct(float *out, float *v1, float *v2, float *v3) {
	out[0] = v1[1] * v2[2] - v1[2] * v2[1];
	out[1] = v1[2] * v2[0] - v1[0] * v2[2];
	out[2] = v1[0] * v2[1] - v1[1] * v2[0];
	return;
}

//m3 is the result, m1 and m2 are the operands
void matrix_multiplyM(float *m3, float *m1, float *m2) {
	int i,j,k;
	float tv1[4], tv2[4];
	for (i = 0;i < 4;i++) {
		k = i * 4;
		tv1[0] = m2[k];
		tv1[1] = m2[k + 1];
		tv1[2] = m2[k + 2];
		tv1[3] = m2[k + 3];
		for (j = 0;j < 4;j++) {
			tv2[0] = m1[j];
			tv2[1] = m1[j + 4];
			tv2[2] = m1[j + 8];
			tv2[3] = m2[j + 12];
			m3[k + j] = matrix_dotproduct(tv1, tv2);
		}
	}
	return;
}

//out and v are vectors, not matrices

void matrix_multiplyV(float *out, float *m, float *v) {
	//I don't know if this is correct... we need to fiddle with v slightly
	float vert[4];
	vert[0] = v[0];
	vert[1] = v[1];
	vert[2] = v[2];
	vert[3] = 1;

	//now, multiply m by vert
	out[0] = vert[0] * m[0] + 
		vert[1] * m[4] + 
		vert[2] * m[8] +
		vert[3] * m[12];
	
	out[1] = vert[0] * m[1] +
		vert[1] * m[5] +
		vert[2] * m[9] +
		vert[3] * m[13];

	out[2] = vert[0] * m[2] +
		vert[1] * m[6] +
		vert[2] * m[10] +
		vert[3] * m[14];
	//last line is blank anyway
	return;
}
/*
void matrix_normalizeV(float *v) {
	float total = (v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]);
	if (!total)
		return;
	total = sqrt(total);
	v[0] /= total;
	v[1] /= total;
	v[2] /= total;
	return;
}
*/
void matrix_rotate(float *m, int axis, float angle) {
	float psin = sin(angle), pcos = cos(angle);
	if (angle) {
		switch(axis) {
			case 2:
				m[0] = pcos;
				m[5] = pcos;
				m[1] = psin;
				m[4] = -psin;
				break;
			case 1:
				m[0] = pcos;
				m[10] = pcos;
				m[2] = -psin;
				m[8] = psin;
				break;
			case 0:
				m[5] = pcos;
				m[6] = psin;
				m[9] = -psin;
				m[10] = pcos;
				break;
		}
	}
	

	return;
}

void matrix_rotate_point(float point[3], float x, float y, float z) {
	float m1[16] = ID_MATRIX, m2[16] = ID_MATRIX, m3[16] = ID_MATRIX,
		m4[16];
	float out[3];

	if (x)
		matrix_rotate(m1, 0, x);

	if (y)
		matrix_rotate(m2, 1, y);

	if (z)
		matrix_rotate(m3, 2, z);

	matrix_multiplyM(m4, m1, m2);
	matrix_multiplyM(m1, m4, m3);

	//now, rotate the point about m1
	matrix_multiplyV(out, m1, point);
	point[0] = out[0];
	point[1] = out[1];
	point[2] = out[2];
	return;
}

void matrix_rotate_around_axis(float *m, float *axis, float angle) {
	float c, s, t;
	c = cos(angle);
	s = sin(angle);
	t = 1-c;

	m[0] = (t * axis[0] * axis[0]) + c;
	m[1] = (t * axis[0] * axis[1]) - (s * axis[2]);
	m[2] = (t * axis[0] * axis[2]) + (s * axis[1]);

	m[4] = (t * axis[0] * axis[1]) + (s * axis[2]);
	m[5] = (t * axis[1] * axis[1]) + c;
	m[6] = (t * axis[1] * axis[2]) - (s * axis[0]);

	m[8] = (t * axis[0] * axis[2]) - (s * axis[1]);
	m[9] = (t * axis[1] * axis[2]) + (s * axis[0]);
	m[10] =(t * axis[2] * axis[2]) + c;

	return;
}

/*
void matrix_print(float *matrix) {
	printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
			matrix[0], matrix[4], matrix[8], matrix[12],
			matrix[1], matrix[5], matrix[9], matrix[13],
			matrix[2], matrix[6], matrix[10], matrix[14],
			matrix[3], matrix[7], matrix[11], matrix[15]);
	return;
}
*/
