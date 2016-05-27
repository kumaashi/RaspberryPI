#include "hwio.h"
#include "uart.h"

void usleep(int c) {
	while(c < 0) {
		c--;
	}
}

// "Collage" COLLAGE.S3M
// written by Necros / FM 
// for the musicdisk
// "Progression"
// (C) 1995 Necros / FM
volatile extern const uint8_t test_wave[];

void main() {
	uart_init();
	
	IO_WRITE(GPIO_SEL4, GPIO_FSEL0_ALT0 | GPIO_FSEL5_ALT0 );
	usleep(10);
	int idiv = 11;
	IO_WRITE(CM_PWMCTL, PM_PASSWORD | CM_KILL);
	usleep(150);
	IO_WRITE(CM_PWMDIV, PM_PASSWORD | (idiv << 12) );
	IO_WRITE(CM_PWMCTL, PM_PASSWORD | CM_ENAB | CM_SRC_PLLDPER);
	usleep(150);

	int range      = 1024;
	int samplerate = 500000000.0 / idiv / range; //PLLD
	usleep(10);
	IO_WRITE(PWM_RNG1, range);
	IO_WRITE(PWM_RNG2, range);
	usleep(10);
	IO_WRITE(PWM_CTL,  PWM_PWEN1 | PWM_PWEN2 | PWM_USEF1 | PWM_USEF2 | PWM_CLRF1);
	
	uint32_t count = 0;
	uint32_t phase = 0;
	while(1) {
		uint32_t offset = (phase >> 8) & 0xFFFFFFFE;
		uint8_t data1 = test_wave[offset + 0];
		uint8_t data2 = test_wave[offset + 1];
		phase += 512;
		IO_WRITE(PWM_FIF1, data1);
		while(IO_READ(PWM_STA) & PWM_FULL1) {}
		IO_WRITE(PWM_FIF1, data2);
		while(IO_READ(PWM_STA) & PWM_FULL1) {}
		count++;
		if((count % 0x1000) == 0) {
			uart_debug_puts("data1=\n", data1);
		}
	}
}

