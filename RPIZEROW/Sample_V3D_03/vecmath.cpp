
extern "C" {
#include "common.h"
#include "tmath.h" 
#include "uart.h" 
#include "vecmath.h" 
}

//ref : https://github.com/unia-sik/emsbench/blob/master/embedded/tg/sqrtf.c
float sqrtf_quake3( float x ) {
	const float xhalf = 0.5f * x;
	union {
		float x;
		int i;
	} u;
	u.x = x;
	u.i = 0x5f3759df - ( u.i >> 1 );
	return x * u.x * (1.5f - xhalf * u.x * u.x);
}

vec4 vec4_set(float x, float y, float z, float w) {
	vec4 a;
	a.v[0] = x;
	a.v[1] = y;
	a.v[2] = z;
	a.v[3] = w;
	return a;
}

float vec4_dot(vec4 &v0, vec4 &v1) {
	return
		v0.v[0] * v1.v[0] +
		v0.v[1] * v1.v[1] +
		v0.v[2] * v1.v[2];
}

float vec4_length(vec4 v) {
	float a = sqrtf_quake3(vec4_dot(v, v));
	return a;
}

vec4 vec4_normalize(vec4 &v)
{
	float len = vec4_length(v);
	if (len == 0)
		len = 1.0f;
	len = 1.0f / len;
	return vec4_set(
			v.v[0] * len,
			v.v[1] * len,
			v.v[2] * len,
			1.0);
}

vec4 vec4_cross(vec4 &a, vec4 &b)
{
	return vec4_set(
			a.v[1] * b.v[2] - a.v[2] * b.v[1],
			a.v[2] * b.v[0] - a.v[0] * b.v[2],
			a.v[0] * b.v[1] - a.v[1] * b.v[0],
			0.0f);
}

void vec4_uart_puts(vec4 & v) {
	uart_puts("------------------------------------------------------\n");
	uart_debug_puts("v.x:", *(uint32_t *)&v.v[0]);
	uart_debug_puts("v.y:", *(uint32_t *)&v.v[1]);
	uart_debug_puts("v.z:", *(uint32_t *)&v.v[2]);
	uart_debug_puts("v.w:", *(uint32_t *)&v.v[3]);
	uart_puts("------------------------------------------------------\n");
}

void matrix_uart_puts(matrix & m) {
	uart_puts("------------------------------------------------------\n");
	uart_debug_puts("ret.m[0 + 4 * 0]=", *(uint32_t *)&m.m[0 + 4 * 0]);
	uart_debug_puts("ret.m[1 + 4 * 0]=", *(uint32_t *)&m.m[1 + 4 * 0]);
	uart_debug_puts("ret.m[2 + 4 * 0]=", *(uint32_t *)&m.m[2 + 4 * 0]);
	uart_debug_puts("ret.m[3 + 4 * 0]=", *(uint32_t *)&m.m[3 + 4 * 0]);
	uart_debug_puts("ret.m[0 + 4 * 1]=", *(uint32_t *)&m.m[0 + 4 * 1]);
	uart_debug_puts("ret.m[1 + 4 * 1]=", *(uint32_t *)&m.m[1 + 4 * 1]);
	uart_debug_puts("ret.m[2 + 4 * 1]=", *(uint32_t *)&m.m[2 + 4 * 1]);
	uart_debug_puts("ret.m[3 + 4 * 1]=", *(uint32_t *)&m.m[3 + 4 * 1]);
	uart_debug_puts("ret.m[0 + 4 * 2]=", *(uint32_t *)&m.m[0 + 4 * 2]);
	uart_debug_puts("ret.m[1 + 4 * 2]=", *(uint32_t *)&m.m[1 + 4 * 2]);
	uart_debug_puts("ret.m[2 + 4 * 2]=", *(uint32_t *)&m.m[2 + 4 * 2]);
	uart_debug_puts("ret.m[3 + 4 * 2]=", *(uint32_t *)&m.m[3 + 4 * 2]);
	uart_debug_puts("ret.m[0 + 4 * 3]=", *(uint32_t *)&m.m[0 + 4 * 3]);
	uart_debug_puts("ret.m[1 + 4 * 3]=", *(uint32_t *)&m.m[1 + 4 * 3]);
	uart_debug_puts("ret.m[2 + 4 * 3]=", *(uint32_t *)&m.m[2 + 4 * 3]);
	uart_debug_puts("ret.m[3 + 4 * 3]=", *(uint32_t *)&m.m[3 + 4 * 3]);
	uart_puts("------------------------------------------------------\n");
}

vec4 matrix_vec4_mult(vec4 & v, matrix & m)
{
	return vec4_set(
			v.v[0] * m.m[0x0] + v.v[1] * m.m[0x4] + v.v[2] * m.m[0x8] + v.v[3] * m.m[0xC],
			v.v[0] * m.m[0x1] + v.v[1] * m.m[0x5] + v.v[2] * m.m[0x9] + v.v[3] * m.m[0xD],
			v.v[0] * m.m[0x2] + v.v[1] * m.m[0x6] + v.v[2] * m.m[0xA] + v.v[3] * m.m[0xE],
			v.v[0] * m.m[0x3] + v.v[1] * m.m[0x7] + v.v[2] * m.m[0xB] + v.v[3] * m.m[0xF]);
}

matrix matrix_ident()
{
	matrix ret;
	ret.m[0 + 4 * 0] = 1;
	ret.m[1 + 4 * 0] = 0;
	ret.m[2 + 4 * 0] = 0;
	ret.m[3 + 4 * 0] = 0;
	ret.m[0 + 4 * 1] = 0;
	ret.m[1 + 4 * 1] = 1;
	ret.m[2 + 4 * 1] = 0;
	ret.m[3 + 4 * 1] = 0;
	ret.m[0 + 4 * 2] = 0;
	ret.m[1 + 4 * 2] = 0;
	ret.m[2 + 4 * 2] = 1;
	ret.m[3 + 4 * 2] = 0;
	ret.m[0 + 4 * 3] = 0;
	ret.m[1 + 4 * 3] = 0;
	ret.m[2 + 4 * 3] = 0;
	ret.m[3 + 4 * 3] = 1;
	return ret;
}

matrix matrix_translate(float x, float y, float z)
{
	matrix ret = matrix_ident();
	ret.m[0 * 4 * 3] = x;
	ret.m[1 * 4 * 3] = y;
	ret.m[2 * 4 * 3] = z;
	return ret;
}

matrix matrix_rotationf(float x, float y, float z)
{
	matrix m = matrix_ident();
	volatile float sin_a = tsin(x);
	volatile float cos_a = tcos(x);
	volatile float sin_b = tsin(y);
	volatile float cos_b = tcos(y);
	volatile float sin_c = tsin(z);
	volatile float cos_c = tcos(z);
	int debug = 0;
	m.m[0 + 0] = cos_b * cos_c;
	m.m[0 + 1] = -cos_b * sin_c;
	m.m[0 + 2] = sin_b;
	m.m[4 + 0] = sin_a * sin_b * cos_c + cos_a * sin_c;
	m.m[4 + 1] = -sin_a * sin_b * sin_c + cos_a * cos_c;
	m.m[4 + 2] = -sin_a * cos_b;
	m.m[8 + 0] = -cos_a * sin_b * cos_c + sin_a * sin_c;
	m.m[8 + 1] = cos_a * sin_b * sin_c + sin_a * cos_c;
	m.m[8 + 2] = cos_a * cos_b;
	return m;
}

matrix matrix_mult(matrix &a, matrix &b)
{
	matrix m = matrix_ident();
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			int yi = y * 4;
			float value = 0;
			value += b.m[yi + 0] * a.m[x + 0];
			value += b.m[yi + 1] * a.m[x + 4];
			value += b.m[yi + 2] * a.m[x + 8];
			value += b.m[yi + 3] * a.m[x + 12];
			m.m[x + y * 4] = value;
		}
	}
	return m;
}


matrix matrix_lookat(
		float eyex, float eyey, float eyez,
		float centerx, float centery, float centerz,
		float upx, float upy, float upz)
{
	vec4 Z = vec4_set(
		centerx - eyex,
		centery - eyey,
		centerz - eyez, 0);
	Z = vec4_normalize(Z);

	vec4 Y = vec4_set(upx, upy, upz, 0);
	vec4 X = vec4_cross(Y, Z);
	X = vec4_normalize(X);
	Y = vec4_cross(Z, X);

	vec4 pos = vec4_set(eyex, eyey, eyez, 0);
	matrix ret = matrix_ident();

	ret.m[4 * 0 + 0] = X.v[0];
	ret.m[4 * 0 + 1] = Y.v[0];
	ret.m[4 * 0 + 2] = Z.v[0];
	ret.m[4 * 0 + 3] = 0;
	ret.m[4 * 1 + 0] = X.v[1];
	ret.m[4 * 1 + 1] = Y.v[1];
	ret.m[4 * 1 + 2] = Z.v[1];
	ret.m[4 * 1 + 3] = 0;
	ret.m[4 * 2 + 0] = X.v[2];
	ret.m[4 * 2 + 1] = Y.v[2];
	ret.m[4 * 2 + 2] = Z.v[2];
	ret.m[4 * 2 + 3] = 0;
	ret.m[4 * 3 + 0] = -vec4_dot(X, pos);
	ret.m[4 * 3 + 1] = -vec4_dot(Y, pos);
	ret.m[4 * 3 + 2] = -vec4_dot(Z, pos);
	ret.m[4 * 3 + 3] = 1;
	return ret;
}

matrix matrix_projection(float fovy, float aspect, float znear, float zfar)
{
	/*
	float tanhfov = 1.0f / ttan(fovy * 0.5);
	float a = 1.0f / (aspect * tanhfov);
	float b = 1.0f / tanhfov;
	float c = (zfar + znear) / (zfar - znear);
	float d = 1.0f;
	float e = -(2 * zfar * znear) / (zfar - znear);

	matrix ret = matrix_ident();
	ret.m[0 + 4 * 0] = a;
	ret.m[1 + 4 * 0] = 0;
	ret.m[2 + 4 * 0] = 0;
	ret.m[3 + 4 * 0] = 0;

	ret.m[0 + 4 * 1] = 0;
	ret.m[1 + 4 * 1] = b;
	ret.m[2 + 4 * 1] = 0;
	ret.m[3 + 4 * 1] = 0;

	ret.m[0 + 4 * 2] = 0;
	ret.m[1 + 4 * 2] = 0;
	ret.m[2 + 4 * 2] = c;
	ret.m[3 + 4 * 2] = d;

	ret.m[0 + 4 * 3] = 0;
	ret.m[1 + 4 * 3] = 0;
	ret.m[2 + 4 * 3] = e;
	ret.m[3 + 4 * 3] = 0;
	*/
	/*
	float tanhfov = 1.0f / ttan(fovy * 0.5);
	float a = 1.0f / (aspect * tanhfov);
	float b = 1.0f / tanhfov;
	float c = (zfar + znear) / (zfar - znear);
	float d = 1.0f;
	float e = -(2 * zfar * znear) / (zfar - znear);
	*/
	//https://docs.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovlh
	float yscale = 1.0 / ttan(fovy * 0.5);
	float xscale = yscale * aspect;
	float Q = zfar / (zfar - znear);
	float W = (-znear * zfar) / (zfar - znear);

	matrix ret = matrix_ident();
	ret.m[4 * 0 + 0] = xscale;
	ret.m[4 * 0 + 1] = 0;
	ret.m[4 * 0 + 2] = 0;
	ret.m[4 * 0 + 3] = 0;

	ret.m[4 * 1 + 0] = 0;
	ret.m[4 * 1 + 1] = yscale;
	ret.m[4 * 1 + 2] = 0;
	ret.m[4 * 1 + 3] = 0;

	ret.m[4 * 2 + 0] = 0;
	ret.m[4 * 2 + 1] = 0;
	ret.m[4 * 2 + 2] = Q;
	ret.m[4 * 2 + 3] = 1;

	ret.m[4 * 3 + 0] = 0;
	ret.m[4 * 3 + 1] = 0;
	ret.m[4 * 3 + 2] = W;
	ret.m[4 * 3 + 3] = 0;
	return ret;
}
