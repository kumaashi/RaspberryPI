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
static const float tbl[] = {
#include "sintable.h"
};

static float *sintbl;

#define WIDTH     640
#define HEIGHT    480
#define BUFNUM    2

#define SOUND_BUF_NUM 4
typedef struct sound_control_t {
	int32_t index;
	int32_t index_max;
	
	void *buffer[16];
	int32_t buffer_size;
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

static const int notetable[] = {
0x00000010, 0x00000011, 0x00000013, 0x00000014,
0x00000015, 0x00000016, 0x00000017, 0x00000019,
0x0000001A, 0x0000001C, 0x0000001E, 0x00000020,
0x00000021, 0x00000023, 0x00000026, 0x00000028,
0x0000002A, 0x0000002D, 0x0000002F, 0x00000032,
0x00000035, 0x00000039, 0x0000003C, 0x00000040,
0x00000043, 0x00000047, 0x0000004C, 0x00000050,
0x00000055, 0x0000005A, 0x0000005F, 0x00000065,
0x0000006B, 0x00000072, 0x00000078, 0x00000080,
0x00000087, 0x0000008F, 0x00000098, 0x000000A1,
0x000000AA, 0x000000B5, 0x000000BF, 0x000000CB,
0x000000D7, 0x000000E4, 0x000000F1, 0x00000100,
0x0000010F, 0x0000011F, 0x00000130, 0x00000142,
0x00000155, 0x0000016A, 0x0000017F, 0x00000196,
0x000001AE, 0x000001C8, 0x000001E3, 0x00000200,
0x0000021E, 0x0000023E, 0x00000260, 0x00000285,
};

volatile int32_t counter = 0;
volatile int32_t phase = 0;
volatile int32_t delta_phase = 0;
volatile uint32_t notes = 0;

int callback_sound_control(void *data) {
	if(*IRQ_BASIC & (1 << 8) && *IRQ_PEND1 & (1 << 16)) {
		//clear int mask DREQ
		*DMA_CS(0) |= (1 << 0);

		static int count = 0;
		count++;
		led_set(count & 1);
		sound_control *pctrl = (sound_control *)data;
		int32_t index = pctrl->index;

		int32_t buffer_size = pctrl->buffer_size;
		int32_t dwcount = buffer_size / 4;
		uint32_t *packetbuffer = (uint32_t *)pctrl->buffer[index];
		float pan[2] =
		{
			0.2, 0.8
		};

		counter++;
		if(counter > 64) {
			phase = 0;
			counter = 0;
			notes++;
			if(notes > 60) {
				notes = 0;
			}
			//delta_phase = notetable[random() & 0x3F];
			delta_phase = notetable[notes];
			uart_debug_puts("change notes=", delta_phase);
			uart_debug_puts("notes=", notes);
		}

		//todo only memcpy
		for(int i = 0 ; i < dwcount; i += 2) {
			phase += delta_phase;
			if( phase >= SIN_TABLE_MAX ) {
				phase -= SIN_TABLE_MAX;
			}
			int32_t ph = phase;

			float value = sintbl[ph % SIN_TABLE_MAX] * 20000.0f;
			uint32_t data = FTOI(value) & 0xFFFF;

			data <<= 16;
			data >>= 4;
			data &= ~0xF;
			packetbuffer[i + 0] = data;
			packetbuffer[i + 1] = data;
		}
		//increase
		pctrl->index = (pctrl->index + 1) % SOUND_BUF_NUM;
	}
	return 0;
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

	sintbl = (float *)heap_1M_get();
	memcpy(sintbl,  tbl, sizeof(tbl));


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
		//dma_debug(0);
		SLEEP(0x400000);
		count++;
	}
	return(0);
}

