// Raspberry Pi Zero ARMv6 frame  buffer
// 2022 yasai kumaashi (gyaboyan@gmail.com)
#include "common.h"
#include "uart.h"
#include "hw.h"
#include "led.h"

#define VCADDR_BASE         0x40000000
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
		MemoryBarrier();
		uint32_t status = *MAILBOX_STATUS;
		uart_debug_puts("READ : MAILBOX_STATUS = ", status);
		if(status & MAILBOX_EMPTY)
			continue;

		MemoryBarrier();
		result = *MAILBOX_READ;
		uart_debug_puts("READ : MAILBOX_READ result = ", result);
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
	fb->vheight = fb->height;
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

int notmain(void) {
	led_init();
	uart_init();
	uart_puts("call notmain\n");
	uart_puts("call fb init\n");
	mailbox_fb_init(640, 480);
	uart_puts("call fb init done\n");

	int count = 0;
	while(1)
	{
		mailbox_fb *fb = mailbox_fb_getaddr();
		uint32_t *p = (uint32_t *)fb->pointer;
		for(int y = 0 ; y < fb->height; y++)
			for(int x = 0 ; x < fb->width; x++)
				*p++ = (x + count) ^ y;
		count++;
	}

	return(0);
}

