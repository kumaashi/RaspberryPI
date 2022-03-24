#include "common.h"
#include "uart.h"
#include "mailbox.h"

static uint32_t ArmToVc(void *p) {
	return ((uint32_t)p) + VCADDR_BASE;
}

static uint32_t VcToArm(uint32_t p) {
	return (uint32_t)(p) & ~(VCADDR_BASE) ;
}

static volatile uint8_t mailbox_data[1024] __attribute__ ((aligned (256))) ;
static volatile mailbox_fb mailbox_fb_data __attribute__((aligned(256))) ;

mailbox_fb * mailbox_fb_getaddr() {
	return (mailbox_fb *)&mailbox_fb_data;
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


uint32_t mailbox_set_domain_state(int id, uint32_t value)
{
	int i = 0;
	uint32_t *p = (uint32_t *)mailbox_data;

	p[i++] = 0;
	p[i++] = 0x00000000;
	p[i++] = SET_DOMAIN_STATE;
	p[i++] = 0x00000008;
	p[i++] = 0x00000008;
	p[i++] = id;
	p[i++] = value;
	p[i++] = 0x00000000;
	p[0] = i * sizeof(uint32_t);
	mailbox_send(p, i);
	return 0;
}

uint32_t mailbox_set_power_state(int id, uint32_t value)
{
	int i = 0;
	uint32_t *p = (uint32_t *)mailbox_data;
	p[i++] = 0;
	p[i++] = 0x00000000;
	p[i++] = SET_POWER_STATE;
	p[i++] = 0x00000008;
	p[i++] = 0x00000008;
	p[i++] = id;
	p[i++] = value;
	p[i++] = 0x00000000;
	p[0] = i * sizeof(uint32_t);
	mailbox_send(p, i);
	return 0;
}

uint32_t mailbox_get_clock_rate(int id)
{
	int i = 0;
	uint32_t *p = (uint32_t *)(uintptr_t)mailbox_data;

	p[i++] = 0;          // size
	p[i++] = 0x00000000; // process request

	p[i++] = GET_CLOCK_RATE; // (the tag id)
	p[i++] = 0x00000008; // length + 4
	p[i++] = 0x00000004; // size of the data
	p[i++] = id; // request
	p[i++] = 0x00000000; // pad

	p[i++] = 0x00000000; // end tag
	p[0]   = i * sizeof(uint32_t); // actual size
	mailbox_send(p, i);
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
