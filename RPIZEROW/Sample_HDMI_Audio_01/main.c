// Raspberry Pi Zero W HDMI registers
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

void test_hdmi() {
	*HDMI_MAI_CHANNEL_MAP = 0x8;
	*HDMI_MAI_CONFIG = 0x0C000003;
	*HDMI_MAI_FMT = 0x00020700;
	*HDMI_CRP_CFG = 0x01001000;
	*HDMI_MAI_SMP = 0x0D0489A6;
	*HDMI_MAI_THR = 0x08080608;
	//*HDMI_CTS_0 = 0x0001220A;
	//*HDMI_CTS_1 = 0x0001220A;
	//*HDMI_TX_PHY_RESET_CTL = 0x8E000000;
	//int index =0 ;
	//*HDMI_RAM_PACKET(4, index++) = 0x000A0184;
	//*HDMI_RAM_PACKET(4, index++) = 0x00000170;
	//*HDMI_RAM_PACKET(4, index++) = 0x00000000;
	//*HDMI_RAM_PACKET(4, index++) = 0x00000000;
	//*HDMI_RAM_PACKET(4, index++) = 0x00000000;
	//*HDMI_RAM_PACKET(4, index++) = 0x00000000;
	//*HDMI_RAM_PACKET(4, index++) = 0x00000000;
	//*HDMI_RAM_PACKET(4, index++) = 0x00000000;
	//*HDMI_RAM_PACKET(4, index++) = 0x00000000;
	//*HDMI_AUDIO_PACKET_CONFIG = 0x0001001C;
	*HDMI_AUDIO_PACKET_CONFIG = 0x21000003;
	*HDMI_MAI_CTL = 0x00003229;
}

int notmain(void) {
	led_init();
	uart_init();

	mailbox_fb_init(WIDTH, HEIGHT, BUFNUM);
	hdmi_print_regs();
	hd_print_regs();
	test_hdmi();
	hdmi_print_regs();
	hd_print_regs();
	int count = 0;
	{
		mailbox_fb *fb = mailbox_fb_getaddr();
		uint32_t *p = (uint32_t *)fb->pointer;
		for(int y = 0 ; y < HEIGHT; y++)
			for(int x = 0 ; x < WIDTH; x++)
				*p++ = x ^ y;

	}
	while(1)
	{
		led_set(count & 1);
		*HDMI_MAI_DATA = (random() << 16);
		count++;
	}
	return(0);
}

