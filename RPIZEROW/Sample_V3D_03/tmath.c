#include <stddef.h>
#include <stdint.h>
#include "tmath.h"
#include "uart.h"

extern int32_t FTOI(float a);

float vsintable[SIN_TABLE_MAX] = {
	#include "sintable.h"
};

float tsin(float a) {
	uint32_t index = FTOI(a * SIN_MN_DIV);
	return vsintable[index & SIN_TABLE_MAX_MASK];
}

float tcos(float a) {
	uint32_t index = FTOI(a * SIN_MN_DIV);
	index += (SIN_TABLE_MAX / 4);
	return vsintable[index & SIN_TABLE_MAX_MASK];
}

float ttan(float a) {
	return tsin(a) / tcos(a);
}
