#include "v3d.h"
#include "uart.h"
#include "util.h"

#include "testdriver.h"

#define SCREEN_WIDTH        640
#define SCREEN_HEIGHT       480

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
	} else {
		uart_puts(" NG\n");
	}
	uart_puts("-------------------------------------------------\n");
	
	
	uint32_t size  = mailbox_fb_init(SCREEN_WIDTH, SCREEN_HEIGHT);
	mailbox_fb *fb = mailbox_fb_getaddr();
	if(fb->pointer == 0) {
		uart_puts(" NG fb->pointer == 0\n");
	}
	uart_puts("GET FRAMEBUFFER\n");
	
	uart_debug_puts("fb->width  =\n", fb->width);
	uart_debug_puts("fb->height =\n", fb->height);
	uart_debug_puts("fb->size   =\n", size);
	uart_debug_puts("fb->pointer=\n", fb->pointer);

	IO_WRITE(V3D_L2CACTL, 0x1);
	uart_debug_puts("V3D_L2CACTL=\n", IO_READ(V3D_L2CACTL));

	
	int count = 0;
	int x, y;
	uint32_t *ptr = (uint32_t *)VcToArm(fb->pointer);
	for(y = 1 ; y < fb->height; y++) {
		for(x = 1 ; x < fb->width; x++) {
			if(!(x % 16) || !(y % 16)) ptr[x + y * fb->width] = 0xFF00FFFF;
		}
	}
	count++;
	
	testTriangle(fb);
	return 0;
}

