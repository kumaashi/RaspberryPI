
#include "uart.h"
#include "mailbox.h"
#include <stdint.h>
#include <math.h>
#include "util.h"

#define OBJECT_MAX  1024
#define WORK_MAX    128
#define PROP_MAX    4
#define PROP_WORLD  0
#define PROP_VIEW   1
#define PROP_PROJ   2

__attribute__((aligned(256))) float  V0[WORK_MAX][4];   ;

__attribute__((aligned(256))) float  MM[OBJECT_MAX][16];  ;
__attribute__((aligned(256))) float  Mt[OBJECT_MAX][16];  ;
__attribute__((aligned(256))) float  Mr[OBJECT_MAX][16];  ;
__attribute__((aligned(256))) float  Ms[OBJECT_MAX][16];  ;
__attribute__((aligned(256))) float  MP[PROP_MAX][16];

/*
skatch


*/
//---------------------------------------------------------
// SET
//---------------------------------------------------------
void Vs_Set(float *p, float s) {
	for(int i = 0 ; i < 4; i++) {
		p[i] = s;
	}
}

//---------------------------------------------------------
// SET
//---------------------------------------------------------
void Vs_Set4(float *p, float a, float b, float c, float d) {
	p[0] = a;
	p[1] = b;
	p[2] = c;
	p[3] = d;
}

//---------------------------------------------------------
// ADD
//---------------------------------------------------------
void VVs_Add4(float *p, float *a, float s) {
	for(int i = 0 ; i < 4; i++) {
		p[i] = a[i] + s;
	}
}

void VVs_Add3(float *p, float *a, float s) {
	for(int i = 0 ; i < 3; i++) {
		p[i] = a[i] + s;
	}
}


//---------------------------------------------------------
// SUB
//---------------------------------------------------------
void VVs_Sub4(float *p, float *a, float s) {
	for(int i = 0 ; i < 4; i++) {
		p[i] = a[i] - s;
	}
}

void VVs_Sub3(float *p, float *a, float s) {
	for(int i = 0 ; i < 3; i++) {
		p[i] = a[i] - s;
	}
}

//---------------------------------------------------------
// MUL
//---------------------------------------------------------
void VVs_Mul4(float *p, float *a, float s) {
	for(int i = 0 ; i < 4; i++) {
		p[i] = a[i] * s;
	}
}

void VVs_Mul3(float *p, float *a, float s) {
	for(int i = 0 ; i < 3; i++) {
		p[i] = a[i] * s;
	}
}

//---------------------------------------------------------
// DIV
//---------------------------------------------------------
void VVs_Div4(float *p, float *a, float s) {
	for(int i = 0 ; i < 4; i++) {
		p[i] = a[i] / s;
	}
}

void VVs_Div3(float *p, float *a, float s) {
	for(int i = 0 ; i < 3; i++) {
		p[i] = a[i] / s;
	}
}


//---------------------------------------------------------
// ADD
//---------------------------------------------------------
void VVV_Add4(float *p, float *a, float *b) {
	for(int i = 0 ; i < 4; i++) {
		p[i] = a[i] + b[i];
	}
}

void VVV_Add3(float *p, float *a, float *b) {
	for(int i = 0 ; i < 3; i++) {
		p[i] = a[i] + b[i];
	}
}


//---------------------------------------------------------
// SUB
//---------------------------------------------------------
void VVV_Sub4(float *p, float *a, float *b) {
	for(int i = 0 ; i < 4; i++) {
		p[i] = a[i] - b[i];
	}
}

void VVV_Sub3(float *p, float *a, float *b) {
	for(int i = 0 ; i < 3; i++) {
		p[i] = a[i] - b[i];
	}
}

//---------------------------------------------------------
// MUL
//---------------------------------------------------------
void VVV_Mul4(float *p, float *a, float *b) {
	for(int i = 0 ; i < 4; i++) {
		p[i] = a[i] * b[i];
	}
}

void VVV_Mul3(float *p, float *a, float *b) {
	for(int i = 0 ; i < 3; i++) {
		p[i] = a[i] * b[i];
	}
}

//---------------------------------------------------------
// DIV
//---------------------------------------------------------
void VVV_Div4(float *p, float *a, float *b) {
	for(int i = 0 ; i < 4; i++) {
		p[i] = a[i] / b[i];
	}
}

void VVV_Div3(float *p, float *a, float *b) {
	for(int i = 0 ; i < 3; i++) {
		p[i] = a[i] / b[i];
	}
}

float VV_Dot(float *a, float *b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

float V_Length(float *a) {
	float s = (a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	return _fsqrt(&s);
}

void V_Normalize(float *a) {
	float s = (a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	float len = 1.0f / _fsqrt(&s);
	a[0] *= len;
	a[1] *= len;
	a[2] *= len;
}


void VVV_Cross(float *p, float *a, float *b) {
	p[0] = a[1] * b[2] - a[2] * b[1];
	p[1] = a[2] * b[0] - a[0] * b[2];
	p[2] = a[0] * b[1] - a[1] * b[0];
}


void MI_Ident(float *p, int idx) {
	p[0]  = 1.0f; p[1]  = 0.0f; p[2]  = 0.0f; p[3]  = 0.0f;
	p[4]  = 0.0f; p[5]  = 1.0f; p[6]  = 0.0f; p[7]  = 0.0f;
	p[8]  = 0.0f; p[9]  = 0.0f; p[10] = 1.0f; p[11] = 0.0f;
	p[12] = 0.0f; p[13] = 0.0f; p[14] = 0.0f; p[15] = 1.0f;
}

void VMV_Mult(float dest[4], float m[16], float p[4]) {
	dest[0] = m[ 0] * p[0] +  m[1] * p[1] +  m[2] * p[2] +  m[3] * p[3];
	dest[1] = m[ 4] * p[0] +  m[5] * p[1] +  m[6] * p[2] +  m[7] * p[3];
	dest[2] = m[ 8] * p[0] +  m[9] * p[1] + m[10] * p[2] + m[11] * p[3];
	dest[3] = m[12] * p[0] + m[13] * p[1] + m[14] * p[2] + m[15] * p[3];
}

void MMM_Mul(float dest[16], const float A[16], const float B[16])
{
	for(int y = 0;y < 4; y++) {
		for(int x = 0;x < 4; x++) {
			dest[y * 4 + x] =
				A[0 + (4 * y)] * B[ 0 + x] + 
				A[1 + (4 * y)] * B[ 4 + x] +
				A[2 + (4 * y)] * B[ 8 + x] +
				A[3 + (4 * y)] * B[12 + x];
		}
	}
}

void Msss_Trans(float *p, float x, float y, float z) {
	p[12] = x; p[13] = y; p[14] = z;
}

void Msss_Scale(float *p, float x, float y, float z) {
	p[0] = x; p[5] = y; p[10] = z;
}


void MIsss_Proj(float *p, int32_t deg, float aspect, float znear, float zfar) {
	float tanparam = FSIN(deg) / FCOS(deg);
	float t = (float)(1.0 / tanparam);
	p[0] = t / aspect;
	p[1] = 0;
	p[2] = 0;
	p[3] = 0;
	
	p[4] = 0;
	p[5] = t;
	p[6] = 0;
	p[7] = 0;
		
	p[8] = 0;
	p[9] = 0;
	p[10] = (zfar + znear) / (znear - zfar);
	p[11] = -1;

	p[12] = 0;
	p[13] = 0;
	p[14] = (2 * zfar * znear) / (znear - zfar);
	p[15] = 0;
}


void MVVV_LookAt(float *p, float *eye, float *center, float *up) {
	enum {
		Eye = 0,
		Center,
		Up,
		X_AXIS,
		Y_AXIS,
		Z_AXIS,
	};
	Vs_Set4(V0[Eye],    eye[0],     eye[1],    eye[2], 0.0);
	Vs_Set4(V0[Center], center[0],  center[1], center[2], 0.0);
	Vs_Set4(V0[Up],      up[0],     up[1],     up[2], 0.0);

	VVV_Sub3(V0[Z_AXIS], V0[Eye], V0[Center]);
	V_Normalize(V0[Z_AXIS]);

	VVV_Cross(V0[X_AXIS], V0[Up], V0[Z_AXIS]);

	VVV_Cross(V0[Y_AXIS], V0[Z_AXIS], V0[X_AXIS]);
	V_Normalize(V0[X_AXIS]);
	V_Normalize(V0[Y_AXIS]);
	p[ 0] = V0[X_AXIS][0];
	p[ 1] = V0[Y_AXIS][0];
	p[ 2] = V0[Z_AXIS][0];
	p[ 3] = 0;

	p[ 4] = V0[X_AXIS][1];
	p[ 5] = V0[Y_AXIS][1];
	p[ 6] = V0[Z_AXIS][1];
	p[ 7] = 0;

	p[ 8] = V0[X_AXIS][2];
	p[ 9] = V0[Y_AXIS][2];
	p[10] = V0[Z_AXIS][2];
	p[11] = 0;
	
	p[12] = -VV_Dot(V0[X_AXIS], V0[Eye]);
	p[13] = -VV_Dot(V0[Y_AXIS], V0[Eye]);
	p[14] = -VV_Dot(V0[Z_AXIS], V0[Eye]);
	p[15] = 1;
}

void MTest(float *p) {
	for(int i = 0 ; i < 16; i++) {
		p[i] = 1.0f * (float)i;
	}
}

void MPrint(float *p) {
	for(int i = 0 ; i < 16; i++) {
		uint32_t k = (uint32_t)p[i];
		uart_debug_puts("M=\n", k);
	}
}

void MInit() {
	for(int i = 0 ; i < OBJECT_MAX; i++) {
		MI_Ident(MM[i], i);
	}
	for(int i = 0 ; i < OBJECT_MAX; i++) {
		//MMM_Mul(MM[i], Mt[i], Mr[i]);
		MMM_Mul_NEON(MM[i], Mt[i], Mr[i]);
	}
	MTest(Mt[0]);
	MTest(Mr[0]);
	MMM_Mul(MM[0], Mt[0], Mr[0]);
	//MMM_Mul_NEON(MM[0], Mt[0], Mr[0]);
	MPrint(MM[0]);
	Vs_Set4(V0[0], 1.0f, 2.0f, 3.0f, 0.0f);
	Vs_Set4(V0[1], 3.0f, 4.0f, 5.0f, 0.0f);
	
	float a = 3;
	float b = 4;
	float c = 5;
	
	float len = (a * a + b * b + c * c);
	//len = _fsqrt(&len);
	len = V_Length(V0[1]);
	
	uart_debug_puts("V_Length=\n", *(uint32_t *)&len);
}

int main() {
	uart_init();
	uart_puts("-------------------------------------------------\n");
	uart_puts("CORE0 READY\n");
	uart_puts("-------------------------------------------------\n");
	uart_puts(" NEON TEST\n");
	uart_puts("-------------------------------------------------\n");
	
	uint32_t start = get_systime_ms();
	MInit();
	uart_debug_puts("TIME[ms]=\n", get_systime_ms() - start);
	uart_debug_puts("ADDR[0]=\n", Mt[0]);
	uart_debug_puts("ADDR[1]=\n", Mt[1]);
	uart_puts(" NEON DONE\n");
	while(1) 
	{
	}
	
	return 0;
}

