// Raspberry Pi Zero W ARMv6 Framebuffer and DMA sample
// 2022 yasai kumaashi (gyaboyan@gmail.com)
// ref:https://github.com/dwelch67/raspberrypi-zero/

extern "C" {
#include "common.h"
#include "uart.h"
#include "hw.h"
#include "led.h"
#include "dma.h"

void *memcpy(void *a, const void *b, size_t sz);
extern const uint8_t *get_fontdata16x16();
void handle_hang();
void intr_handler();
void __aeabi_unwind_cpp_pr0() {}
int notmain();

} //EXTERN C

void *memcpy(void *a, const void *b, size_t sz) {
	uint8_t *d = (uint8_t *)a;
	uint8_t *s = (uint8_t *)b;
	for(size_t i = 0 ; i < sz; i++)
		*d++ = *s++;
	return a;
}

//https://github.com/raspberrypi/firmware/issues/67
#define IRQ_BASE                                               (SUBSYSTEM_BASE + 0xB200)
#define IRQ_GPU_PENDING1                                       ((volatile uint32_t *)(SUBSYSTEM_BASE + 0xB204))
#define IRQ_GPU_PENDING2                                       ((volatile uint32_t *)(SUBSYSTEM_BASE + 0xB208))
#define IRQ_GPU_ENABLE2                                        ((volatile uint32_t *)(SUBSYSTEM_BASE + 0xB214))
#define IRQ_GPU_FAKE_ISR                                       0x10000
#define SMI_BASE                                               (SUBSYSTEM_BASE + 0x600000)
#define SMI_CS                                                 ((volatile uint32_t *)(SMI_BASE + 0x00))

//fake_vsync_isr=1
void fake_vsync(void) {
	*IRQ_GPU_ENABLE2 = IRQ_GPU_FAKE_ISR;
	while( (*IRQ_GPU_PENDING2 & IRQ_GPU_FAKE_ISR) == 0 );
	*SMI_CS = 0;
}

#define MAILBOX_EMPTY       0x40000000
#define MAILBOX_FULL        0x80000000
#define MAILBOX_BASE        (SUBSYSTEM_BASE + 0xB880)
#define MAILBOX_READ        ((volatile uint32_t *)(MAILBOX_BASE + 0x00))
#define MAILBOX_STATUS      ((volatile uint32_t *)(MAILBOX_BASE + 0x18))
#define MAILBOX_WRITE       ((volatile uint32_t *)(MAILBOX_BASE + 0x20))
#define MAILBOX_FRAMEBUFFER 1
#define MAILBOX_VC_TO_ARM   9
#define MAILBOX_ARM_TO_VC   8

typedef struct mailbox_fb_t {
	uint32_t width;
	uint32_t height;
	uint32_t vwidth;
	uint32_t vheight;
	uint32_t pitch;
	uint32_t depth;
	uint32_t x;
	uint32_t y;
	uint32_t pointer;
	uint32_t size;
	uint32_t pointer_vc;
} mailbox_fb;

uint32_t ArmToVc(void *p)
{
	return ((uint32_t)p) + VCADDR_BASE;
}

uint32_t VcToArm(uint32_t p) {
	return (uint32_t)(p) & ~(VCADDR_BASE) ;
}

static volatile uint8_t mailbox_data[1024] __attribute__ ((aligned (256))) ;
static volatile mailbox_fb mailbox_fb_data __attribute__((aligned(256))) ;

mailbox_fb * mailbox_fb_getaddr() {
	return (mailbox_fb *)&mailbox_fb_data;
}

void fb_debug() {
	mailbox_fb * fb = mailbox_fb_getaddr();

	uart_debug_puts("fb->width =", fb->width);
	uart_debug_puts("fb->heigh =", fb->height);
	uart_debug_puts("fb->vwidt =", fb->vwidth);
	uart_debug_puts("fb->vheig =", fb->vheight);
	uart_debug_puts("fb->pitch =", fb->pitch);
	uart_debug_puts("fb->depth =", fb->depth);
	uart_debug_puts("fb->x     =", fb->x    );
	uart_debug_puts("fb->y     =", fb->y    );
	uart_debug_puts("fb->point =", fb->pointer);
	uart_debug_puts("fb->size  =", fb->size );
}

void mailbox_write(uint32_t mask, uint32_t v) {
	InvalidateData();
	while(1) {
		InvalidateData();
		uint32_t status  = *MAILBOX_STATUS;
		if(status & MAILBOX_FULL)
			continue;
		break;
	}
	InvalidateData();
	uint32_t value = (mask | (v & 0xFFFFFFF0));
	*MAILBOX_WRITE = value;
}

uint32_t mailbox_read(uint32_t mask) {
	uint32_t result = 0;
	while(1) {
		InvalidateData();
		uint32_t status = *MAILBOX_STATUS;
		//uart_debug_puts("READ : MAILBOX_STATUS = ", status);
		if(status & MAILBOX_EMPTY)
			continue;

		InvalidateData();
		result = *MAILBOX_READ;
		//uart_debug_puts("READ : MAILBOX_READ result = ", result);
		if( (result & 0xF) == mask)
			break;
	}
	return result & 0xFFFFFFF0;
}

uint32_t mailbox_send(uint32_t *p, int len)
{
	int count = 100;
	do {
		mailbox_write(MAILBOX_ARM_TO_VC, (uint32_t)(p) + 0xC0000000);
		mailbox_read(MAILBOX_ARM_TO_VC);
		//uart_debug_puts("SEND mailbox read:", p[1]);

		InvalidateData();
		if (p[1] == 0x80000000)
			break;
	} while (count-- > 0);
	return p[1];
}

int32_t mailbox_fb_init(uint32_t w, uint32_t h, uint32_t bufnum) {
	volatile mailbox_fb *fb = mailbox_fb_getaddr();
	uart_debug_puts("fb struct addr=", (uint32_t)fb);
	fb->width   = w;
	fb->height  = h;
	fb->vwidth  = fb->width;
	fb->vheight = fb->height * bufnum;
	fb->pitch   = 0;
	fb->depth   = 32;
	fb->x       = 0;
	fb->y       = 0;
	fb->pointer = 0;
	fb->size    = 0;

	fb->pointer_vc = 0;

	int count = 0;
	do {
		mailbox_write(MAILBOX_FRAMEBUFFER, ArmToVc((void *)fb));
		mailbox_read(MAILBOX_FRAMEBUFFER);
		if(fb->pointer) {
			break;
		}
		count++;
		SLEEP(0x1000);
	} while(1);
	uint32_t vc_pointer = fb->pointer;
	fb->pointer = VcToArm(fb->pointer);
	fb->pointer_vc = vc_pointer;
	return 0;
}

uint32_t mailbox_fb_flip(int index)
{
	int i = 0;
	volatile mailbox_fb *fb = mailbox_fb_getaddr();
	uint32_t *p = (uint32_t *)mailbox_data;

	p[i++] = 0;          // size
	p[i++] = 0x00000000; // process request
	p[i++] = SET_VIRTUAL_OFFSET; // (the tag id)
	p[i++] = 0x00000008; // (the tag id)
	p[i++] = 0x00000008; // (the tag id)
	p[i++] = 0;          // V3D
	p[i++] = (fb->height * index);
	p[i++] = 0x00000000; // end tag
	p[i++] = 0x00000000; // end tag
	p[0] = i * sizeof(uint32_t); // actual size
	mailbox_send(p, i);
	return p[1];
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
#define TEX_X     256
#define TEX_Y     256
#define CHAR_SIZE 16
uint32_t tex[TEX_X * TEX_Y] __attribute__ ((aligned (32))) ;
extern uint32_t surface[WIDTH * HEIGHT];

void init_tex() {
	const uint8_t *p = get_fontdata16x16();
	for(int j = 0 ; j < TEX_Y; j++) {
		for(int i = 0 ; i < TEX_X; i++) {
			tex[i + j * TEX_X] = 0;
			if(p[i + j * TEX_X])
				tex[i + j * TEX_X] = 0xFFFFFFFF;
			if((i % 16) == 0)
				tex[i + j * TEX_X] = 0xFFFFFFFF;
			if((j % 16) == 0)
				tex[i + j * TEX_X] = 0xFFFFFFFF;
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



//test
int random() {
	static int a = 1;
	static int b = 234567;
	static int c = 8715623;

	a += b;
	b += c;
	c += a;
	return (a >> 16);
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
	dst += ((tx * char_size + (random() & 0x3)) * 1) * 4;
	dst += ((ty * char_size + (random() & 0x3)) * width) * 4;

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
	//init peri
	led_init();
	uart_init();
	dma_init();

	init_tex();
	int bufnum = 4;
	mailbox_fb_init(WIDTH, HEIGHT, bufnum);

	int count = 0;
	while(1)
	{
		auto fb = mailbox_fb_getaddr();

		/*
		const uint32_t color_table[4] = {
			0xFFFF0000,
			0xFFFFFF00,
			0xFFFF00FF,
			0xFF00FFFF,
		};
		dma_clear_framebuffer((uint32_t)surface, fb->width, fb->height, color_table[count % 4]);
		*/
		dma_clear_framebuffer((uint32_t)surface, fb->width, fb->height, 0x11122334);


		//Rendering letter atlas
		{
			dma_control_block *p = dma_get_cb();
			dma_cb_set_ti_src_inc(p, 1);
			dma_cb_set_ti_dst_inc(p, 1);
			dma_cb_set_ti_tdmode(p, 1);
			dma_cb_set_txfr_len_xlength(p, TEX_X * 4);
			dma_cb_set_txfr_len_ylength(p, TEX_Y - 1);

			dma_cb_set_stride_d(p, (WIDTH * 4) - (TEX_X * 4));
			dma_cb_set_addr(p, (count % WIDTH * 4) + (uint32_t)surface + fb->width * 200, (uint32_t)tex);
		}

		//Draw Test Strings
		/*
		dma_draw_str(
			(uint32_t)surface,
			"#include<stdio.h>\nint main() {\n    printf(\"Hello world\");\n    return 0;\n}", 0, 0, fb->width);
		*/

		dma_draw_str((uint32_t)surface, "DMA01 Sample for Raspberry PI Zero W", 0, 0, fb->width);
		dma_draw_str((uint32_t)surface, "Applied fake vsync isr", 0, 20, fb->width);
		dma_draw_str((uint32_t)surface, "flip uses set virtual offset", 0, 22, fb->width);
		dma_draw_str((uint32_t)surface, "2022 GYABO ", 0, 28, fb->width);

		//DMA0 Submit to objects.
		dma_submit_cb(0);

		//Present
		{
			uint32_t ptr = (uint32_t)fb->pointer;
			uint32_t src = (uint32_t)surface;
			ptr += (count % bufnum) * fb->width * fb->height * 4;

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
		mailbox_fb_flip(count % bufnum);
		count++;
	}
	return(0);
}

uint32_t surface[WIDTH * HEIGHT]  __attribute__ ((aligned (256))) ;
