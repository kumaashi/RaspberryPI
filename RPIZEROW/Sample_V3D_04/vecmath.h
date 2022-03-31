#ifndef _VECMATH_H_
#define _VECMATH_H_

struct vec4 {
	float v[4];
} __attribute__((__packed__));

struct matrix {
	float m[16];
} __attribute__((__packed__));

//ref : https://github.com/unia-sik/emsbench/blob/master/embedded/tg/sqrtf.c
float sqrtf_quake3( float x );
float vec4_dot(vec4 &v0, vec4 &v1);
float vec4_length(vec4 v);
matrix matrix_ident();
matrix matrix_lookat(
		float eyex, float eyey, float eyez,
		float centerx, float centery, float centerz,
		float upx, float upy, float upz);
matrix matrix_mult(matrix &a, matrix &b);
matrix matrix_projection(float fovy, float aspect, float znear, float zfar);
matrix matrix_rotationf(float x, float y, float z);
matrix matrix_translate(float x, float y, float z);
vec4 matrix_vec4_mult(vec4 & v, matrix & m);
vec4 vec4_cross(vec4 &a, vec4 &b);
vec4 vec4_normalize(vec4 &v);
vec4 vec4_set(float x, float y, float z, float w);
void matrix_uart_puts(matrix & m);
void vec4_uart_puts(vec4 & v);
#endif //_VECMATH_H_

