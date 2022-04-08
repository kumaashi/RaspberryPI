// Raspberry Pi Zero W ARMv6 DWC2 USB tentative driver
// 2022 yasai kumaashi (gyaboyan@gmail.com)
// ref:https://github.com/dwelch67/raspberrypi-zero/
#include "common.h"
#include "uart.h"
#include "hw.h"
#include "irq.h"
#include "led.h"
#include "dma.h"
#include "mailbox.h"
#include "usb.h"
#include "usb_hakopad.h"

#include "heap.h"

extern const uint8_t *get_fontdata16x16();

void common_memset(void *d, uint8_t ch, size_t sz) {
	uint8_t *p = (uint8_t *)d;
	for(size_t i = 0 ; i < sz; i++)
		*p++ = ch;
}

void handle_hang() {
	led_init();
	uart_init();
	while(1) {
		uart_puts("HANG!!!!!!!!!\n");
		SLEEP(0x10000);
	}
}

void intr_handler() {
	uart_puts("call intr_handler\n");
}


#define WIDTH     640
#define HEIGHT    480
#define BUFNUM    2

#define TEX_X     256
#define TEX_Y     256
#define CHAR_SIZE 16

uint32_t *tex;
uint32_t *surface;

int random() {
	static int a = 1;
	static int b = 234567;
	static int c = 8715623;

	a += b;
	b += c;
	c += a;
	return (a >> 16);
}

void init_tex() {
	const uint8_t *p = get_fontdata16x16();
	for(int j = 0 ; j < TEX_Y; j++) {
		for(int i = 0 ; i < TEX_X; i++) {
			tex[i + j * TEX_X] = 0;
			if(p[i + j * TEX_X])
				tex[i + j * TEX_X] = 0xFF00FF00;
		}
	}
}

uint32_t clear_color[4] __attribute__ ((aligned (32))) ;
void dma_clear_framebuffer(uint32_t ptr, int width, int height, uint32_t color)
{
	clear_color[0] = color;
	clear_color[1] = color;
	clear_color[2] = color;
	clear_color[3] = color;

	dma_control_block *p = dma_get_cb();
	dma_cb_set_ti_dst_inc(p, 1);
	dma_cb_set_txfr_len(p, width * height * 4);
	dma_cb_set_addr(p, ptr, (uint32_t)clear_color);
}

void draw_char(uint32_t pointer, int ch, int x, int y, int width) {
	int char_size = CHAR_SIZE;
	int tx = x % 64;
	int ty = y % 64;

	int idx_x = ch % 16;
	int idx_y = ch / 16;
	int start_offset = 4 * (char_size * idx_x + char_size * idx_y * char_size * char_size);

	int char_size_bytes = char_size * 4;
	uint32_t src = (uint32_t)tex + start_offset;
	uint32_t dst = (uint32_t)pointer;
	//dst += ((tx * char_size + (random() & 0x3)) * 1) * 4;
	//dst += ((ty * char_size + (random() & 0x3)) * width) * 4;
	dst += ((tx * char_size) * 1) * 4;
	dst += ((ty * char_size) * width) * 4;

	dma_control_block *p = dma_get_cb();
	dma_cb_set_ti_src_inc(p, 1);
	dma_cb_set_ti_dst_inc(p, 1);
	dma_cb_set_ti_tdmode(p, 1);
	dma_cb_set_txfr_len_xlength(p, char_size_bytes);
	dma_cb_set_txfr_len_ylength(p, char_size - 1);
	dma_cb_set_stride_d(p, (WIDTH * 4) - char_size_bytes);
	dma_cb_set_stride_s(p, (TEX_X * 4) - char_size_bytes);
	dma_cb_set_addr(p, dst, src);
}

void draw_char_imm(uint32_t pointer, int ch, int x, int y, int width) {
	int char_size = CHAR_SIZE;

	int idx_x = ch % 16;
	int idx_y = ch / 16;
	int start_offset = 4 * (char_size * idx_x + char_size * idx_y * char_size * char_size);

	int char_size_bytes = char_size * 4;
	uint32_t src = (uint32_t)tex + start_offset;
	uint32_t dst = (uint32_t)pointer;
	dst += (x * 1) * 4;
	dst += (y * width) * 4;

	dma_control_block *p = dma_get_cb();
	dma_cb_set_ti_src_inc(p, 1);
	dma_cb_set_ti_dst_inc(p, 1);
	dma_cb_set_ti_tdmode(p, 1);
	dma_cb_set_txfr_len_xlength(p, char_size_bytes);
	dma_cb_set_txfr_len_ylength(p, char_size - 1);
	dma_cb_set_stride_d(p, (WIDTH * 4) - char_size_bytes);
	dma_cb_set_stride_s(p, (TEX_X * 4) - char_size_bytes);
	dma_cb_set_addr(p, dst, src);
}

void dma_draw_str(uint32_t pointer, const char *s, int x, int y, int width) {
	int tx = x;
	int ty = y;
	while(*s) {
		int ch = *s++;
		if(ch == ' ') {
			tx++;
			continue;
		}
		if(ch == '\n') {
			ty++;
			tx = x;
			continue;
		}
		draw_char(pointer, ch, tx, ty, width);
		tx++;
	}
}

int notmain(void) {
	typedef struct point_t {
		int32_t x, y;
	} point;

	//init peri
	led_init();
	uart_init();

	//init heap
	heap_init(0x200000);
	tex = (uint32_t *)heap_get();
	surface = (uint32_t *)heap_get();

	//init misc
	init_tex();
	dma_init();


	//init hakopad
	hakopad_init();

	//init fb
	mailbox_fb_init(WIDTH, HEIGHT, BUFNUM);

	int count = 0;
	while(1)
	{
		mailbox_fb *fb = mailbox_fb_getaddr();

		hakopad_data *ppad = hakopad_get_data();

		dma_clear_framebuffer((uint32_t)surface, fb->width, fb->height, 0x00000000);
		int pad_y = 2;
		dma_draw_str((uint32_t)surface, "RPIZERO-W DWC2 USB HAKOPAD DRIVER", 0, pad_y++, fb->width);
		dma_draw_str((uint32_t)surface, "2022 GYABO ", 0, 28, fb->width);

		//LEFT
		point left_p;
		left_p.x = ppad->Left_stick_X_axis / 256;
		left_p.y = -ppad->Left_stick_Y_axis / 256;
		left_p.x += (fb->width / 2);
		left_p.y += (fb->height / 2);

		//RIGHT
		point right_p;
		right_p.x = ppad->Right_stick_X_axis / 256;
		right_p.y = -ppad->Right_stick_Y_axis / 256;
		right_p.x += (fb->width / 2);
		right_p.y += (fb->height / 2);

		//DRAW
		draw_char_imm((uint32_t)surface, 'L', left_p.x, left_p.y, fb->width);
		draw_char_imm((uint32_t)surface, 'R', right_p.x, right_p.y, fb->width);


		if(ppad->D_Pad_up         ) dma_draw_str((uint32_t)surface, "PAD_UP", 0, pad_y++, fb->width);
		if(ppad->D_Pad_down       ) dma_draw_str((uint32_t)surface, "PAD_DOWN", 0, pad_y++, fb->width);
		if(ppad->D_Pad_left       ) dma_draw_str((uint32_t)surface, "PAD_LEFT", 0, pad_y++, fb->width);
		if(ppad->D_pad_right      ) dma_draw_str((uint32_t)surface, "PAD_RIGHT", 0, pad_y++, fb->width);
		if(ppad->Start_button     ) dma_draw_str((uint32_t)surface, "BUTTON_START", 0, pad_y++, fb->width);
		if(ppad->Back_button      ) dma_draw_str((uint32_t)surface, "BUTTON_BACK", 0, pad_y++, fb->width);
		if(ppad->Left_stick_press ) dma_draw_str((uint32_t)surface, "STICK_LEFT", 0, pad_y++, fb->width);
		if(ppad->Right_stick_press) dma_draw_str((uint32_t)surface, "STICK_RIGHT", 0, pad_y++, fb->width);
		if(ppad->Button_LB        ) dma_draw_str((uint32_t)surface, "BUTTON_LB", 0, pad_y++, fb->width);
		if(ppad->Button_RB        ) dma_draw_str((uint32_t)surface, "BUTTON_RB", 0, pad_y++, fb->width);
		if(ppad->Xbox_logo_button ) dma_draw_str((uint32_t)surface, "BUTTON HAKO LOGO", 0, pad_y++, fb->width);
		if(ppad->Unused           ) dma_draw_str((uint32_t)surface, "Unused", 0, pad_y++, fb->width);
		if(ppad->Button_A         ) dma_draw_str((uint32_t)surface, "BUTTON A", 0, pad_y++, fb->width);
		if(ppad->Button_B         ) dma_draw_str((uint32_t)surface, "BUTTON B", 0, pad_y++, fb->width);
		if(ppad->Button_X         ) dma_draw_str((uint32_t)surface, "BUTTON X", 0, pad_y++, fb->width);
		if(ppad->Button_Y         ) dma_draw_str((uint32_t)surface, "BUTTON Y", 0, pad_y++, fb->width);

		//active atmark
		draw_char_imm((uint32_t)surface, '@', count % WIDTH, pad_y++ , fb->width);

		//unsigned Left_trigger          : 8;
		//unsigned Right_trigger         : 8;
		draw_char_imm((uint32_t)surface,
			'T', (WIDTH / 3) * 1, HEIGHT - (CHAR_SIZE * 2) - ((ppad->Left_trigger * HEIGHT) >> 9), fb->width);
		draw_char_imm((uint32_t)surface,
			'T', (WIDTH / 3) * 2, HEIGHT - (CHAR_SIZE * 2) - ((ppad->Right_trigger * HEIGHT) >> 9), fb->width);

		//DMA0 Submit to objects.
		dma_submit_cb(0);

		//Present
		{
			uint32_t ptr = (uint32_t)fb->pointer;
			uint32_t src = (uint32_t)surface;
			ptr += (count % BUFNUM) * fb->width * fb->height * 4;

			dma_control_block *p = dma_get_cb();
			dma_cb_set_ti_src_inc(p, 1);
			dma_cb_set_ti_dst_inc(p, 1);
			dma_cb_set_txfr_len(p, WIDTH * HEIGHT * 4);
			dma_cb_set_addr(p, ptr, src);
		}

		//VSYNC
		fake_vsync();

		//DMA0 Wait for write to the surface.
		dma_wait(0);

		//DMA1 Submit trans from surface to backbuffer.
		dma_submit_cb(1);

		//DMA1 Wait for write to the backbuffer
		dma_wait(1);

		//Flip
		mailbox_fb_flip(count % BUFNUM);
		count++;

		//update gamepad
		hakopad_update();
	}
	return(0);
}
