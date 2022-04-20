// Raspberry Pi Zero W HDMI Audio DMA Streaming
// 2022 yasai kumaashi (gyaboyan@gmail.com)
#include "common.h"
#include "heap.h"
#include "uart.h"
#include "hw.h"
#include "led.h"
#include "mailbox.h"
#include "dma.h"
#include "irq.h"
#include "hdmiaudio.h"

#define SIN_TABLE_MAX  4096
float tbl[] = {
#include "sintable.h"
};

#define WIDTH     640
#define HEIGHT    480
#define BUFNUM    2

#define SOUND_BUF_NUM 4
typedef struct sound_control_t {
	int index;
	int index_max;
	
	void *buffer[16];
	int buffer_size;
} sound_control;

int random() {
	static int a = 1;
	static int b = 234876;
	static int c = 8751273;
	a += b;
	b += c;
	c += a;
	return (a >> 16);
	
}

int callback_sound_control(void *data) {
	if(*IRQ_BASIC & (1 << 8) && *IRQ_PEND1 & (1 << 16)) {
		static int count = 0;
		count++;
		led_set(count & 1);
		sound_control *pctrl = (sound_control *)data;
		int index = pctrl->index;

		int buffer_size = pctrl->buffer_size;
		int dwcount = buffer_size / 4;
		uint32_t *packetbuffer = (uint32_t *)pctrl->buffer[index];
		static int cur = 0;
		float pan[2] =
		{
			0.2, 0.8
		};

		//todo only memcpy
		for(int i = 0 ; i < dwcount; i += 2) {
			uint32_t data = FTOI(tbl[cur % SIN_TABLE_MAX] * 20000.0) & 0xFFFF; //random() & 0xFFFF;

			data <<= 16;
			data >>= 4;
			data &= ~0xF;
			packetbuffer[i + 0] = data;
			packetbuffer[i + 1] = data;
			cur += 10;
		}
		//increase
		pctrl->index = (pctrl->index + 1) % SOUND_BUF_NUM;
		
		//clear int mask DREQ
		*DMA_CS(0) |= (1 << 0);
	}
}

void enable_irq_dreq() {
	*IRQ_ENABLE1 = (1 << 16);
}

int main(void) {
	//init peri
	irq_init();
	led_init();
	uart_init();
	dma_init();

	//init heap
	heap_init(0x2000000);

	//init fb
	mailbox_fb_init(WIDTH, HEIGHT, BUFNUM);
	
	//init audio
	hdmi_print_regs();
	hd_print_regs();
	hdmi_audio_setup();
	hdmi_print_regs();
	hd_print_regs();
	
	mailbox_fb *fb = mailbox_fb_getaddr();
	uint32_t *screen = (uint32_t *)fb->pointer;
	{
		for(int y = 0 ; y < HEIGHT; y++)
			for(int x = 0 ; x < WIDTH; x++)
				screen[x + y * WIDTH] = x ^ y;

	}

	//setup dma and enable of dreq from hdmi signal.
	sound_control ctrl;
	
	//setup irq
	irq_info iirq;
	iirq.data = (void *)&ctrl;
	iirq.callback = callback_sound_control;
	irq_set_info(&iirq, 0);

	//setup sound control
	ctrl.index = 0;
	ctrl.index_max = SOUND_BUF_NUM;
	ctrl.buffer_size = 8192;

	dma_control_block *db = (dma_control_block *)heap_4k_get();
	memset(db, 0, HEAP_4K_SIZE);
	for(int i = 0 ; i < SOUND_BUF_NUM; i++) {
		ctrl.buffer[i] = (void *)heap_1M_get();
		memset(ctrl.buffer[i], 0, HEAP_1M_SIZE);

		dma_control_block *dmadata = &db[i];
		dma_cb_set_addr(dmadata, (uint32_t)hdmi_audio_get_fifo_pointer(), (uint32_t)ctrl.buffer[i]);
		dma_cb_set_txfr_len(dmadata, ctrl.buffer_size);
		dma_cb_set_ti_src_inc(dmadata, 1);
		dma_cb_set_ti_burst_length(dmadata, 2); //todo appropriate 
		dma_cb_set_ti_inten(dmadata, 1);

		//enable dreq from HDMI
		dma_cb_set_ti_dst_dreq(dmadata, 1);
		dma_cb_set_ti_permap(dmadata, DMA_PERMAP_HDMI);

	}


	//todo avoid modop
	for(int i = 0 ; i < ctrl.index_max; i++) {
		dma_cb_set_next_cb(&db[i], &db[(i + 1) % SOUND_BUF_NUM]);
	}
	
	
	//enable IRQ
	enable_irq_dreq();
	ENABLE_IRQ();

	//todo also need a time irq on rendering next buffer?

	//kick
	dma_submit_cb(0, db);

	//infloop
	int count = 0;
	while(1) {
		dma_debug(0);
		SLEEP(0x400000);
		count++;
	}
	return(0);
}

