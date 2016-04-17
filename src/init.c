#include "v3d.h"
#include "led.h"
#include "uart.h"
#include "console.h"


#include <stdint.h>
#include <math.h>

extern void testTriangle(void *dest);


void disp_wakeup_v3d() {
	uint32_t offset = 0;
	for(int j = 0 ; j < 29; j++) {
		draw_dword(IO_READ(0x20C00000 + offset),  0, j);
		offset += 4;
		draw_dword(IO_READ(0x20C00000 + offset),  9, j);
		offset += 4;
		draw_dword(IO_READ(0x20C00000 + offset), 18, j);
		offset += 4;
		draw_dword(IO_READ(0x20C00000 + offset), 27, j);
		offset += 4;
	}
}

int main() {
	uart_init();
	uart_puts("-------------------------------------------------\n");
	uart_puts("CORE0 READY\n");
	uart_puts("-------------------------------------------------\n");
	uart_puts(" V3D TEST\n");
	uart_puts("-------------------------------------------------\n");
	mailbox_qpu_enable();
	if( IO_READ(V3D_IDENT0) == 0x02443356) {
		uart_puts(" OK! BLINK STATUS LED\n");
		mark_status_led();
	} else {
		uart_puts(" NG\n");
		halt_led0();
	}
	uart_puts("-------------------------------------------------\n");
	
	
	uint32_t size  = mailbox_fb_init(SCREEN_WIDTH, SCREEN_HEIGHT);
	mailbox_fb *fb = mailbox_fb_getaddr();
	if(fb->pointer == 0) {
		halt_led0();
	}
	uart_puts("GET FRAMEBUFFER\n");
	
	uart_debug_puts("fb->width  =\n", fb->width);
	uart_debug_puts("fb->height =\n", fb->height);
	uart_debug_puts("fb->size   =\n", size);
	uart_debug_puts("fb->pointer=\n", fb->pointer);

	
	int count = 0;
	int x, y;
	uint32_t *ptr = (uint32_t *)fb->pointer;
	for(y = 1 ; y < fb->height; y++) {
		for(x = 1 ; x < fb->width; x++) {
			ptr[x + y * fb->width] = 0xFFFF0000;
		}
	}
	count++;
	usleep(0x10000);
	uart_puts("TEST HATCH.\n");

	//test driver
	uart_puts("test driver.\n");
	testTriangle(fb->pointer);
	return 0;
}

