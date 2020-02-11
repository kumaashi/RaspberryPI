#include "util.h"
#include "sintable.inl"
#include <math.h>

float fsin(int32_t i) {
	return fsintable[i % FSIN_TABLE_MAX];
}

float fcos(int32_t i) {
	return fsintable[ (i + FSIN_TABLE_MAX_D4 ) % FSIN_TABLE_MAX];
}

static uint32_t rand_a = 1;
static uint32_t rand_b = 23456;
static uint32_t rand_c = 7890123;
void init_random() {
	rand_a = 1;
	rand_b = 23456;
	rand_c = 7890123;
}

uint32_t random() {
	rand_a += rand_b;
	rand_b += rand_c;
	rand_c += rand_a;
	return (rand_a >> 16);
}

void usleep(int32_t count) {
	volatile int32_t k = count;
	while(k > 0) {
		k--;
	}
}

void memset_t(void *dest, uint8_t c, uint32_t size) {
	if(size & 0x3) {
		volatile uint8_t *p = (uint8_t *)dest;
		for(int i = 0 ; i < size; i++) {
			p[i] = c;
		}
	} else {
		volatile uint32_t *p = (uint32_t *)dest;
		size >>= 2;
		uint32_t data = c | c << 8 | c << 16 | c << 24;
		for(int i = 0 ; i < size; i++) {
			p[i] = data;
		}
	}
}


uint64_t get_systime(void) {
	uint64_t t;
	volatile uint32_t chi = IO_READ(SYST_CHI);
	volatile uint32_t clo = IO_READ(SYST_CLO);
	if(chi != IO_READ(SYST_CHI)) {
		chi = IO_READ(SYST_CHI);
		clo = IO_READ(SYST_CLO);
	}
	t  = chi;
	t <<= 32;
	t += clo;
	return t;
}

uint32_t get_systime_ms(void) {
	uint64_t t;
	volatile uint32_t chi = IO_READ(SYST_CHI);
	volatile uint32_t clo = IO_READ(SYST_CLO);
	if(chi != IO_READ(SYST_CHI)) {
		chi = IO_READ(SYST_CHI);
		clo = IO_READ(SYST_CLO);
	}
	t  = chi;
	t <<= 32;
	t += clo;
	return (uint32_t)((float)clo / 1000.0f);
}

volatile inline uint32_t get_systime32(void) {
	return IO_READ(SYST_CLO);
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


