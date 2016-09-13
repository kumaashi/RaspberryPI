#ifndef _NEON_H_
#define _NEON_H_

extern float _fsqrt(float *a);
inline float fsqrt(float a) {
	return _fsqrt(&a);
}

#endif //_NEON_H_


