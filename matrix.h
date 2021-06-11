
/** @file matrix.h
 * Matrix and vector math functions */

#ifndef MATRIX_H
#define MATRIX_H

//create an identity matrix... remember to free it later
/** Allocate and fill in a 4x4 identity matrix. */
void matrix_create_identity(float *m);

/** Build a rotation matrix. */
float *matrix_create_rotation(int axis, float angle);


/** Move a matrix around */
void matrix_translate(float *m, float x, float y, float z);

/** Add src to dst, leave in dst (4x4) */
void matrix_add(float *dst, float *src);

void matrix_normalizeV(float *vec);

/** Find the dot product of 2 matrices. 
 * m1 and m2 are arrays of *4* elements each. */
inline float matrix_dotproduct(const float *m1, const float *m2);

/** Find the cross product of 2 vectors (3x1) */
void matrix_crossproduct(float *out, float *v1, float *v2);

void matrix_normal(float *out, float *v1, float *v2, float *v3);

/** Multiply a matrix by a scalar */
void matrix_multiplyS(float *m, float s);

/** multiply a matrix by another matrix */
//m3 = m1 * m2
void matrix_multiplyM(float *m3, float *m1, float *m2);

/** multiply a vector by a matrix
 * out = m * v
 * v and out are 3 elements
 */
void matrix_multiplyV(float *out, float *m, float *v);

/* rotate the point by the matrix (possibly translate as well).
 * point is replaced by the result.
 * point is 3 floats.
 */
void matrix_rotate_point(float point[], float x, float y, float z);

void matrix_rotate_around_axis(float m[], float axis[], float angle);

void matrix_rotate(float m[], int axis, float angle);

#endif

