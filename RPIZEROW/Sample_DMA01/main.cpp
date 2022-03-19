// Raspberry Pi Zero W ARMv6 DWC2 USB tentative driver
// 2022 yasai kumaashi (gyaboyan@gmail.com)
// ref:https://github.com/dwelch67/raspberrypi-zero/

extern "C" {
#include "common.h"
#include "uart.h"
#include "hw.h"
#include "led.h"

	void *memcpy(void *a, const void *b, size_t sz) {
		uint8_t *d = (uint8_t *)a;
		uint8_t *s = (uint8_t *)b;
		for(size_t i = 0 ; i < sz; i++)
			*d++ = *s++;
		return a;
	}
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
	*SMI_CS = 0;
	while( (*IRQ_GPU_PENDING2 & IRQ_GPU_FAKE_ISR) == 0 );
}

//#define VCADDR_BASE       0xC0000000 //for L2
#define MAILBOX_EMPTY       0x40000000
#define MAILBOX_FULL        0x80000000
#define MAILBOX_BASE        (SUBSYSTEM_BASE + 0xB880)
#define MAILBOX_READ        ((volatile uint32_t *)(MAILBOX_BASE + 0x00))
#define MAILBOX_STATUS      ((volatile uint32_t *)(MAILBOX_BASE + 0x18))
#define MAILBOX_WRITE       ((volatile uint32_t *)(MAILBOX_BASE + 0x20))
#define MAILBOX_FRAMEBUFFER 1
#define MAILBOX_VC_TO_ARM   9
#define MAILBOX_ARM_TO_VC   8

#define MAILBOX_SET_VIRTUAL_OFFSET    0x00048009 // Frame Buffer: Set Virtual Offset (Response: X In Pixels, Y In Pixels)

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

	uint32_t
ArmToVc(void *p)
{
	return ((uint32_t)p) + VCADDR_BASE;
}

uint32_t
VcToArm(uint32_t p) {
	return (uint32_t)(p) & ~(VCADDR_BASE) ;
}

static volatile uint8_t mailbox_data[1024] __attribute__ ((aligned (256))) ;
static volatile mailbox_fb mailbox_fb_data __attribute__((aligned(256))) ;

mailbox_fb *
mailbox_fb_getaddr() {
	return (mailbox_fb *)&mailbox_fb_data;
}

void
mailbox_write(uint32_t mask, uint32_t v) {
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

uint32_t
mailbox_read(uint32_t mask) {
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

uint32_t
mailbox_send(uint32_t *p, int len)
{
	int count = 100;
	do {
		mailbox_write(MAILBOX_ARM_TO_VC, (uint32_t)(p) + 0xC0000000);
		mailbox_read(MAILBOX_ARM_TO_VC);
		uart_debug_puts("SEND mailbox read:", p[1]);

		InvalidateData();
		if (p[1] == 0x80000000)
			break;
	} while (count-- > 0);
	return p[1];
}

int32_t
mailbox_fb_init(uint32_t w, uint32_t h) {
	volatile mailbox_fb *fb = mailbox_fb_getaddr();
	uart_debug_puts("fb struct addr=", (uint32_t)fb);
	fb->width   = w;
	fb->height  = h;
	fb->vwidth  = fb->width;
	fb->vheight = fb->height * 2;
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

uint32_t
mailbox_fb_flip(int index)
{
	int i = 0;
	volatile mailbox_fb *fb = mailbox_fb_getaddr();
	uint32_t *p = (uint32_t *)mailbox_data;

	p[i++] = 0;          // size
	p[i++] = 0x00000000; // process request
	p[i++] = MAILBOX_SET_VIRTUAL_OFFSET; // (the tag id)
	p[i++] = 0x00000008; // (the tag id)
	p[i++] = 0x00000008; // (the tag id)
	p[i++] = 0;          // V3D
	p[i++] = fb->height * index;
	p[i++] = 0x00000000; // end tag
	p[i++] = 0x00000000; // end tag
	p[0] = i * sizeof(uint32_t); // actual size
	mailbox_send(p, i);
	return p[1];
}


extern "C" {

void __aeabi_unwind_cpp_pr0() {}

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


#define DMA_BASE                   (0x20007000)
#define DMA_CS(x)                  ((volatile uint32_t *)(DMA_BASE + 0x00 + (0x100 * (x))))
#define DMA_CB_ADDR(x)             ((volatile uint32_t *)(DMA_BASE + 0x04 + (0x100 * (x))))
#define DMA_TI(x)                  ((volatile uint32_t *)(DMA_BASE + 0x08 + (0x100 * (x))))
#define DMA_S_ADDR(x)              ((volatile uint32_t *)(DMA_BASE + 0x0c + (0x100 * (x))))
#define DMA_D_ADDR(x)              ((volatile uint32_t *)(DMA_BASE + 0x10 + (0x100 * (x))))
#define DMA_TXFR_LEN(x)            ((volatile uint32_t *)(DMA_BASE + 0x14 + (0x100 * (x))))
#define DMA_STRIDE(x)              ((volatile uint32_t *)(DMA_BASE + 0x18 + (0x100 * (x))))
#define DMA_NEXTCONBK(x)           ((volatile uint32_t *)(DMA_BASE + 0x1c + (0x100 * (x))))
#define DMA_DEBUG(x)               ((volatile uint32_t *)(DMA_BASE + 0x20 + (0x100 * (x))))
#define DMA_INT_STATUS             ((volatile uint32_t *)(DMA_BASE + 0xFE0))
#define DMA_ENABLE                 ((volatile uint32_t *)(DMA_BASE + 0xFF0))

#define DMA_CB_MAX 8192
typedef struct dma_control_block_t {
	uint32_t ti;
	uint32_t s_addr;
	uint32_t d_addr;
	uint32_t txfr_len;

	uint32_t stride;
	uint32_t next_cb;
	uint32_t debug;
	uint32_t reserved;
} __attribute__((__packed__)) dma_control_block;
static_assert(sizeof(dma_control_block) == 32, "DMA CB was not aligned");

dma_control_block g_dma_cb[DMA_CB_MAX]  __attribute__ ((aligned (32))) ;
static int dma_cb_index = 0;

void clear_dma_cb() {
	uint8_t *p = (uint8_t *)g_dma_cb;
	for(unsigned int i = 0; i < sizeof(g_dma_cb); i++)
		*p++ = 0;
	dma_cb_index = 0;
}

dma_control_block *get_dma_cb() {
	dma_control_block * ret = &g_dma_cb[dma_cb_index % DMA_CB_MAX];
	dma_cb_index++;

	ret->next_cb = (uint32_t)(&g_dma_cb[dma_cb_index % DMA_CB_MAX]);
	//uart_debug_puts("ret=", (uint32_t)ret);
	//uart_debug_puts("ret->next_cb=", (uint32_t)ret->next_cb);
	//uart_debug_puts("ret->next_cb=", (uint32_t)ret->next_cb);
	return ret;
}

void submit_dma_cb() {
	*DMA_ENABLE |= (1 << 0);
	*DMA_CS(0) = (1 << 31);
	*DMA_CB_ADDR(0) = (uint32_t)g_dma_cb;
	InvalidateData();
	*DMA_CS(0) |= (1 << 0) | (1 << 29);
	while(*DMA_CS(0) & 0x1) {
		//uart_puts("Wait complete DMA\n");
	}
}

#define CHAR_SIZE 16
#define WIDTH     640
#define HEIGHT    480
#define TEX_X     256
#define TEX_Y     256
uint32_t tex[TEX_X * TEX_Y] __attribute__ ((aligned (32))) ;
uint32_t clear_color[4] __attribute__ ((aligned (32))) ;

extern const uint8_t *get_fontdata16x16();
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

void dma_clear_framebuffer(uint32_t ptr, int width, int height, uint32_t color)
{
	clear_color[0] = color;
	clear_color[1] = color;
	clear_color[2] = color;
	clear_color[3] = color;

	dma_control_block *p = get_dma_cb();
	p->ti = (1 << 4) | (1 << 0); //enable int
	p->txfr_len = width * height * 4;
	p->s_addr = (uint32_t)clear_color;
	p->s_addr += 0x40000000;
	p->d_addr = (uint32_t)ptr;
	p->d_addr += 0x40000000;
}

void draw_char(uint32_t pointer, int ch, int x, int y, int width) {
	int char_size = CHAR_SIZE;
	int tx = x % 64;
	int ty = y % 64;

	//calc ascii
	int idx_x = ch % 16;
	int idx_y = ch / 16;
	int start_offset = 4 * (char_size * idx_x + char_size * idx_y * char_size * char_size);
	int tdmode = 1;

	dma_control_block *p = get_dma_cb();
	p->ti = (1 << 8) | (1 << 4) | (tdmode << 1);
	p->txfr_len = ((char_size - 1) << 16) | (char_size * 4);
	p->stride = (WIDTH * 4) - (char_size * 4);
	p->stride <<= 16;
	p->stride |= (TEX_X * 4) - (char_size * 4);

	p->s_addr = (uint32_t)tex + start_offset;
	p->s_addr += 0x40000000;
	p->d_addr = (uint32_t)pointer;
	p->d_addr += (tx * char_size) * 4;
	p->d_addr += (ty * char_size * width) * 4;
	p->d_addr += 0x40000000;
}

void dma_draw_str(uint32_t pointer, const char *s, int x, int y, int width) {
	int tx = x;
	int ty = y;
	while(*s) {
		int ch = *s++;
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

	init_tex();
	mailbox_fb_init(WIDTH, HEIGHT);

	int count = 0;
	while(1)
	{

		clear_dma_cb();
		auto fb = mailbox_fb_getaddr();
		fb->pointer = fb->pointer_vc;
		fb->pointer += (count % 2) * fb->width * fb->height * 4;
		dma_clear_framebuffer(fb->pointer, fb->width, fb->height, 0xFF0000FF);

		{
			dma_control_block *p = get_dma_cb();
			int tdmode = 1;
			p->ti = (1 << 8) | (1 << 4) | (tdmode << 1);
			p->txfr_len = ((TEX_Y - 1) << 16) | (TEX_X * 4);
			p->stride = (WIDTH * 4) - (TEX_X * 4);
			p->stride <<= 16;

			p->s_addr = (uint32_t)tex;
			p->s_addr += 0x40000000;
			p->d_addr = (uint32_t)fb->pointer + fb->width * 200;;
			p->d_addr += 0x40000000;
			p->d_addr += (count % WIDTH) * 4;
		}

		dma_draw_str(
			fb->pointer, "#include<stdio.h>\nint main() {\n    printf(\"Hello world\");\n    return 0;\n}", 0, 0, fb->width);

		submit_dma_cb();

		fake_vsync();
		mailbox_fb_flip(count % 2);
		count++;
	}
	return(0);
}
}
