// Raspberry Pi Zero W HDMI Audio 02
// 2022 yasai kumaashi (gyaboyan@gmail.com)
// ref:https://github.com/dwelch67/raspberrypi-zero/
#include "common.h"
#include "uart.h"
#include "hw.h"
#include "led.h"
#include "mailbox.h"

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
//https://github.com/raspberrypi/linux/issues/528
//https://github.com/raspberrypi/firmware/issues/183
void hdmi_audio_setup() {

	*HDMI_AUDIO_PACKET_CONFIG = 0x21000003;
	*HDMI_MAI_CHANNEL_MAP = 0x8;
	*HDMI_MAI_CONFIG = 0x0C000003;
	*HDMI_MAI_FMT = 0x00020900;
	*HDMI_MAI_SMP = 0x0DCD21F3;
	*HDMI_CRP_CFG = 0x01001000;
	*HDMI_MAI_CTL |= 0x1; //RESET.0, ENABLE.3, PARITYEN.8, FLUSH.9, WHOLSMP.12, CHALIGN.13
	*HDMI_MAI_CTL = (1 << 3) | (1 << 8) | (1 << 12); ////ENABLE.3, PARITYEN.8, FLUSH.9, WHOLSMP.12, CHALIGN.13
	SLEEP(0x1000000);
}

extern uint32_t ipaaddr[];
extern uint32_t ipaaddr_size[];

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
	int global_count = 0;
	int counter = 0;
	int ch = 0;
	int file_size = 9525644 + 11289724; //todo : (uint32_t)ipaaddr_size;

	volatile uint16_t *wavdata = (uint16_t *)ipaaddr;
	uart_debug_puts("ipaaddr=", (uint32_t)ipaaddr);
	uart_debug_puts("wavdata=", (uint32_t)wavdata);
	while(1) {
		led_set(count & 1);
		{
			//dword
			if(global_count > (file_size / 2)) {
				wavdata = (uint16_t *)ipaaddr;
				global_count = 0;
				counter = 0;
				ch = 0;
			}
			uint32_t data = wavdata[global_count];
			screen[( (global_count >> 8) % WIDTH) + (data >> 8) * WIDTH] ^= 0xFF0FF00F;

			//[preamble][reversd bit][4bit]
			data <<= 16;

			data >>= 4;
			data &= ~0xF;

			int loop_count = 0;
			static int busy_th = 0x2;
			while(*HDMI_MAI_CTL & (1 << 11)) {
				loop_count++;
				if(loop_count > busy_th) {
					loop_count = 0;
					busy_th <<= 1;
					*HDMI_MAI_CTL |= (1 << 2 );
					*HDMI_MAI_CTL |= (1 << 1 );
					uart_puts("BUSY\n");
				}
			}

			//Submit subframe. To od DREQ with DMA.
			*HDMI_MAI_DATA = data;

			global_count++;
			counter++;
			ch++;

			counter %= 192;
			if(counter == 0) {
				static int count = 0;
				count++;
				if(count > 100) {
					uart_debug_puts("HDMI_M_CTL         =", *HDMI_M_CTL       );
					uart_debug_puts("HDMI_MAI_CTL       =", *HDMI_MAI_CTL     );
					count = 0;
				}
			}
			ch %= 2;
		}
		count++;
	}
	return(0);
}

