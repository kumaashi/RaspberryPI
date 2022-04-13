#include "common.h"
#include "hw.h"
#include "led.h"

void led_init() {
	uint32_t val;

	val = GET32(GPFSEL4);
	val &= ~(7 << 21);
	val |= (1 << 21);
	PUT32(GPFSEL4, val);
}

void led_set(int a) {
	if(a) {
		PUT32(GPSET1, 1 << (47 - 32));
	} else {
		PUT32(GPCLR1, 1 << (47 - 32));
	}
}

