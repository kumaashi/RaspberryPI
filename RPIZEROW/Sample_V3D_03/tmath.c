#include "tmath.h"

float vsintable[SIN_TABLE_MAX] = {
	#include "sintable.h"
};

float tsin(float a) {
	return vsintable[(int)(a * SIN_MN_DIV) % SIN_TABLE_MAX];
}

float tcos(float a) {
	return vsintable[((SIN_TABLE_MAX >> 2) + (int)(a * SIN_MN_DIV)) % SIN_TABLE_MAX];
}
