#include "common.h"

uint32_t random() {
	static uint32_t a = 1;
	static uint32_t b = 1235666;
	static uint32_t c = 9786423;
	a += b;
	b += c;
	c += a;
	return (a >> 16);
}

//mimic posix
void *memcpy(void *dst, const void *src, size_t n) {
	uint8_t *d = (uint8_t *)dst;
	uint8_t *s = (uint8_t *)src;
	for(int i = 0; i < n; i++) {
		*d++ = *s++;
	}
	return dst;
}

void *memset(void *dst, int c, size_t n) {
	uint8_t *d = (uint8_t *)dst;
	for(int i = 0; i < n; i++) {
		*d++ = c;
	}
	return dst;
}
