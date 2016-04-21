#include "hwio.h"
#include "util.h"

static int is_init = 0;
void output_enable_status_led() {
	if(is_init == 0) {
		IO_WRITE(GPIO_SEL4, 1 << (3 * 7));
		usleep(1000);
	}
}

static void _hald_led(int count) {
	output_enable_status_led();
	while(1) {
		IO_WRITE(GPIO_SET1, 1 << GPIO_STATUS_LED);
		usleep(count);
		IO_WRITE(GPIO_CLR1, 1 << GPIO_STATUS_LED);
		usleep(count);
	}
}

void halt_led0() {
	_hald_led(50000);
}

void halt_led1() {
	_hald_led(700000);
}

void on_status_led() {
	output_enable_status_led();
	IO_WRITE(GPIO_CLR1, 1 << GPIO_STATUS_LED);
}

void off_status_led() {
	output_enable_status_led();
	IO_WRITE(GPIO_SET1, 1 << GPIO_STATUS_LED);
}


void mark_status_led() {
	int count = 10;
	output_enable_status_led();
	while(count--) {
		IO_WRITE(GPIO_SET1, 1 << GPIO_STATUS_LED);
		usleep(100000);
		IO_WRITE(GPIO_CLR1, 1 << GPIO_STATUS_LED);
		usleep(100000);
	}
}

