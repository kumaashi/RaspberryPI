// Raspberry Pi Zero W HDMI Audio 02
// 2022 yasai kumaashi (gyaboyan@gmail.com)
// ref:https://github.com/dwelch67/raspberrypi-zero/
#include "common.h"
#include "uart.h"
#include "hw.h"
#include "led.h"
#include "mailbox.h"
#include "dma.h"

#define WIDTH     640
#define HEIGHT    480
#define BUFNUM    2

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


uint32_t random() {
	static uint32_t a = 1;
	static uint32_t b = 1235666;
	static uint32_t c = 9786423;
	a += b;
	b += c;
	c += a;
	return (a >> 16);
}

void hd_print_regs() {
	uart_puts("-------------------------------------------\n");
	uart_debug_puts("HDMI_M_CTL         =", *HDMI_M_CTL       );
	uart_debug_puts("HDMI_MAI_CTL       =", *HDMI_MAI_CTL     );
	uart_debug_puts("HDMI_MAI_THR       =", *HDMI_MAI_THR     );
	uart_debug_puts("HDMI_MAI_FMT       =", *HDMI_MAI_FMT     );
	uart_debug_puts("HDMI_MAI_DATA      =", *HDMI_MAI_DATA    );
	uart_debug_puts("HDMI_MAI_SMP       =", *HDMI_MAI_SMP     );
	uart_debug_puts("HDMI_VID_CTL       =", *HDMI_VID_CTL     );
	uart_debug_puts("HDMI_CSC_CTL       =", *HDMI_CSC_CTL     );
	uart_debug_puts("HDMI_CSC_12_11     =", *HDMI_CSC_12_11   );
	uart_debug_puts("HDMI_CSC_14_13     =", *HDMI_CSC_14_13   );
	uart_debug_puts("HDMI_CSC_22_21     =", *HDMI_CSC_22_21   );
	uart_debug_puts("HDMI_CSC_24_23     =", *HDMI_CSC_24_23   );
	uart_debug_puts("HDMI_CSC_32_31     =", *HDMI_CSC_32_31   );
	uart_debug_puts("HDMI_CSC_34_33     =", *HDMI_CSC_34_33   );
	uart_debug_puts("HDMI_FRAME_COUNT   =", *HDMI_FRAME_COUNT );
	uart_puts("-------------------------------------------\n");
}

void hdmi_print_regs() {
	uart_puts("-------------------------------------------\n");
	uart_debug_puts("HDMI_CORE_REV             =",              *HDMI_CORE_REV            );
	uart_debug_puts("HDMI_SW_RESET_CONTROL     =",              *HDMI_SW_RESET_CONTROL    ); 
	uart_debug_puts("HDMI_HOTPLUG_INT          =",              *HDMI_HOTPLUG_INT         ); 
	uart_debug_puts("HDMI_HOTPLUG              =",              *HDMI_HOTPLUG             ); 
	uart_debug_puts("HDMI_FIFO_CTL             =",              *HDMI_FIFO_CTL            ); 
	uart_debug_puts("HDMI_MAI_CHANNEL_MAP      =",              *HDMI_MAI_CHANNEL_MAP     ); 
	uart_debug_puts("HDMI_MAI_CONFIG           =",              *HDMI_MAI_CONFIG          ); 
	uart_debug_puts("HDMI_MAI_FORMAT           =",              *HDMI_MAI_FORMAT          ); 
	uart_debug_puts("HDMI_AUDIO_PACKET_CONFIG  =",              *HDMI_AUDIO_PACKET_CONFIG ); 
	uart_debug_puts("HDMI_RAM_PACKET_CONFIG    =",              *HDMI_RAM_PACKET_CONFIG   ); 
	uart_debug_puts("HDMI_RAM_PACKET_STATUS    =",              *HDMI_RAM_PACKET_STATUS   ); 
	uart_debug_puts("HDMI_CRP_CFG              =",              *HDMI_CRP_CFG             ); 
	uart_debug_puts("HDMI_CTS_0                =",              *HDMI_CTS_0               ); 
	uart_debug_puts("HDMI_CTS_1                =",              *HDMI_CTS_1               ); 
	uart_debug_puts("HDMI_SCHEDULER_CONTROL    =",              *HDMI_SCHEDULER_CONTROL   ); 
	uart_debug_puts("HDMI_HORZA                =",              *HDMI_HORZA               ); 
	uart_debug_puts("HDMI_HORZB                =",              *HDMI_HORZB               ); 
	uart_debug_puts("HDMI_VERTA0               =",              *HDMI_VERTA0              ); 
	uart_debug_puts("HDMI_VERTB0               =",              *HDMI_VERTB0              ); 
	uart_debug_puts("HDMI_VERTA1               =",              *HDMI_VERTA1              ); 
	uart_debug_puts("HDMI_VERTB1               =",              *HDMI_VERTB1              ); 
	uart_debug_puts("HDMI_CEC_CNTRL_1          =",              *HDMI_CEC_CNTRL_1         ); 
	uart_debug_puts("HDMI_CEC_CNTRL_2          =",              *HDMI_CEC_CNTRL_2         ); 
	uart_debug_puts("HDMI_CEC_CNTRL_3          =",              *HDMI_CEC_CNTRL_3         ); 
	uart_debug_puts("HDMI_CEC_CNTRL_4          =",              *HDMI_CEC_CNTRL_4         ); 
	uart_debug_puts("HDMI_CEC_CNTRL_5          =",              *HDMI_CEC_CNTRL_5         ); 
	uart_debug_puts("HDMI_CEC_TX_DATA_1        =",              *HDMI_CEC_TX_DATA_1       ); 
	uart_debug_puts("HDMI_CEC_TX_DATA_2        =",              *HDMI_CEC_TX_DATA_2       ); 
	uart_debug_puts("HDMI_CEC_TX_DATA_3        =",              *HDMI_CEC_TX_DATA_3       ); 
	uart_debug_puts("HDMI_CEC_TX_DATA_4        =",              *HDMI_CEC_TX_DATA_4       ); 
	uart_debug_puts("HDMI_CEC_RX_DATA_1        =",              *HDMI_CEC_RX_DATA_1       ); 
	uart_debug_puts("HDMI_CEC_RX_DATA_2        =",              *HDMI_CEC_RX_DATA_2       ); 
	uart_debug_puts("HDMI_CEC_RX_DATA_3        =",              *HDMI_CEC_RX_DATA_3       ); 
	uart_debug_puts("HDMI_CEC_RX_DATA_4        =",              *HDMI_CEC_RX_DATA_4       ); 
	uart_debug_puts("HDMI_TX_PHY_RESET_CTL     =",              *HDMI_TX_PHY_RESET_CTL    ); 
	uart_debug_puts("HDMI_TX_PHY_CTL_0         =",              *HDMI_TX_PHY_CTL_0        ); 
	uart_debug_puts("HDMI_CEC_CPU_STATUS       =",              *HDMI_CEC_CPU_STATUS      ); 
	uart_debug_puts("HDMI_CEC_CPU_SET          =",              *HDMI_CEC_CPU_SET         ); 
	uart_debug_puts("HDMI_CEC_CPU_CLEAR        =",              *HDMI_CEC_CPU_CLEAR       ); 
	uart_debug_puts("HDMI_CEC_CPU_MASK_STATUS  =",              *HDMI_CEC_CPU_MASK_STATUS ); 
	uart_debug_puts("HDMI_CEC_CPU_MASK_SET     =",              *HDMI_CEC_CPU_MASK_SET    ); 
	uart_debug_puts("HDMI_CEC_CPU_MASK_CLEAR   =",              *HDMI_CEC_CPU_MASK_CLEAR  ); 
	uart_debug_puts("HDMI_RAM_PACKET_START     =",              *HDMI_RAM_PACKET_START    ); 
	uart_puts("-------------------------------------------\n");
}



//https://github.com/raspberrypi/firmware/issues/222
//https://github.com/raspberrypi/firmware/issues/222
//https://github.com/raspberrypi/linux/issues/528
//https://github.com/raspberrypi/firmware/issues/183
void hdmi_audio_setup() {
	*HDMI_AUDIO_PACKET_CONFIG = 0x21002003;
	*HDMI_MAI_CHANNEL_MAP = 0x8;
	*HDMI_MAI_CONFIG = 0x0C000003;
	*HDMI_MAI_FMT = 0x00020900;

	*HDMI_MAI_SMP = 0x0DCD21F3;
	//*HDMI_MAI_SMP = 0x0D0489A6; //smp8000hz

	//*HDMI_MAI_THR = 0x08080608;
	*HDMI_MAI_THR = 0x04040404;
	*HDMI_CRP_CFG = 0x01001000;
	*HDMI_MAI_CTL = 0x00003329; //RESET.0, ENABLE.3, PARITYEN.8, FLUSH.9, WHOLSMP.12, CHALIGN.13
}

extern uint32_t ipaaddr[];
extern uint32_t ipaaddr_size[];
extern uint32_t sound_buffer[];

int notmain(void) {
	led_init();
	uart_init();

	mailbox_fb_init(WIDTH, HEIGHT, BUFNUM);
	hdmi_print_regs();
	hd_print_regs();
	hdmi_audio_setup();
	hdmi_print_regs();
	hd_print_regs();
	int count = 0;
	mailbox_fb *fb = mailbox_fb_getaddr();
	uint32_t *screen = (uint32_t *)fb->pointer;
	{
		for(int y = 0 ; y < HEIGHT; y++)
			for(int x = 0 ; x < WIDTH; x++)
				screen[x + y * WIDTH] = x ^ y;

	}
	int file_size = (uint32_t)ipaaddr_size;
	int sound_buffer_size = file_size * 2; //uint16 -> uint32 (STD subframe)

	uart_puts("Start wav to STD(LE) subpacket to subpacket...");
	for(int i = 0 ; i < file_size / 2; i++) {
		uint16_t *wavdata = (uint16_t *)ipaaddr;
		uint32_t data = wavdata[i];
		data <<= 16;

		//subframe bits
		data >>= 4;
		data &= ~0xF;
		sound_buffer[i] = data;
	}
	uart_puts("Done.\n");

	//setup dma and enable of dreq from hdmi signal.
	{
		dma_init();

		dma_control_block *dmadata = dma_get_cb();
		dma_cb_set_addr(dmadata, (uint32_t)HDMI_MAI_DATA_BUS, (uint32_t)sound_buffer);
		dma_cb_set_ti_src_inc(dmadata, 1);
		dma_cb_set_ti_burst_length(dmadata, 2);
		dma_cb_set_txfr_len(dmadata, sound_buffer_size);

		//enable dreq from HDMI
		dma_cb_set_ti_dst_dreq(dmadata, 1);
		dma_cb_set_ti_permap(dmadata, DMA_PERMAP_HDMI);

		//make ring link
		dma_control_block *dmadata2 = dma_get_cb();
		dma_cb_dup(dmadata2, dmadata);
		dmadata2->next_cb = dmadata;

		//kick
		dma_submit_cb(0);
	}


	//infloop
	while(1) {
		led_set(count & 1);
		dma_debug(0);
		SLEEP(0x400000);
		count++;
	}
	return(0);
}

