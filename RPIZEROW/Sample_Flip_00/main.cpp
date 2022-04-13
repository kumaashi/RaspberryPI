// For Raspberry Pi Zero W V3D test
// 2022 yasai kumaashi (gyaboyan@gmail.com)

extern "C" {
#include "common.h"
#include "uart.h"
#include "hw.h"
#include "led.h"
#include "mailbox.h"
#include "irq.h"
} //EXTERN C

#define WIDTH             640
#define HEIGHT            480
#define BUFNUM            2
#define TILE_SIZE         32

int maincpp(void) {
	led_init();
	uart_init();


	mailbox_fb_init(WIDTH, HEIGHT, BUFNUM);
	mailbox_fb *fb = mailbox_fb_get();
	surface_t *screen0 = (surface_t *)fb->pointer;
	surface_t *screen1 = screen0 + (WIDTH * HEIGHT);
	for(int y = 0 ; y < HEIGHT; y++) {
		for(int x = 0 ; x < WIDTH; x++) {
			screen0[x + y * WIDTH] = 0xFFFF0000;
			screen1[x + y * WIDTH] = 0xFF0000FF;
		}
	}
	int count = 0;
	while(1) {
		uart_debug_puts("count=", count);
		int frame_index = count & 1;
		while(count && get_irq_flip_count() != count) {
		}
		led_set(count & 1);
		//fake_vsync();
		ENABLE_IRQ();
		enable_irq_flip_isr(frame_index, count + 1);
		SLEEP(1000);
		count++;
	}
	return(0);
}


extern "C" {
	int notmain() {
		maincpp();
		return 0;
	}
}
