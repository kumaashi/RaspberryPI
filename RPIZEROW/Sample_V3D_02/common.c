#include "common.h"
#include "hw.h"

uint32_t common_random() {
	static uint32_t a = 1;
	static uint32_t b = 1235666;
	static uint32_t c = 9786423;
	a += b;
	b += c;
	c += a;
	return (a >> 16);
}

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

uint64_t get_systime(void) {
	uint64_t t;
	uint32_t chi = *SYST_CHI;
	uint32_t clo = *SYST_CLO;
	t = chi;
	t <<= 32;
	t += clo;
	return t;
}

uint32_t get_systime_ms(void) {
	uint64_t t;
	uint32_t chi = *SYST_CHI;
	uint32_t clo = *SYST_CLO;
	t  = chi;
	t <<= 32;
	t += clo;
	return (uint32_t)((float)clo / 1000.0f);
}

uint32_t get_systime32(void) {
	return *SYST_CLO;
}

void msleep(uint32_t ms) {
	uint64_t at = get_systime() + ms * 1000;
	while(get_systime() < at);
	return;
}

void msleep32(uint32_t ms) {
	uint32_t at = get_systime32() + ms * 1000;
	while(get_systime32() < at);
	return;
}


fps_info show_fps_raw() {
	static fps_info info;
	static uint32_t last = 0;
	static uint32_t frames = 0;
	static uint32_t dt = 0;
	uint32_t current = get_systime_ms();
	frames++;
	if(10000 <= (current - last)) {
		dt = current - last;
		last = current;
		info.dt = dt;
		info.frames = frames;
		frames = 0;
	}
	return info;
}

float show_fps() {
	fps_info info = show_fps_raw();
	float dt = (float)(info.dt) / 10000.0f;
	return (float)info.frames / dt;
}

