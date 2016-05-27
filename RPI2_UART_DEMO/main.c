#include "hwio.h"
#include "uart.h"

static volatile void busy_wait(int32_t count) {
	while(count > 0) {
		count--;
	}
}

void main() {
	uart_init();
	while(1) {
		uart_dump(VCHIQ_BASE, 0x100);
		busy_wait(0x1000000);
	}
}

