
#include "common.h"
#include "hw.h"
#include "uart.h"

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

uint32_t hdmi_audio_get_fifo_pointer() {
	return (uint32_t)HDMI_MAI_DATA_BUS;
}

void hdmi_audio_stop_packet(int isforce) {
	return;
	uart_puts("hdmi_audio_stop_packet\n");
	*HDMI_RAM_PACKET_CONFIG &= ~(1 << 4);
	if(isforce)
		return;
	while(1) {
		if(*HDMI_RAM_PACKET_STATUS & (1 << 4))
			break;
	};
}

void hdmi_audio_start_packet(int isforce) {
	uart_puts("hdmi_audio_start_packet\n");
	*HDMI_RAM_PACKET_CONFIG |= (1 << 4);
	if(isforce)
		return;
	while(1) {
		if(*HDMI_RAM_PACKET_STATUS & (1 << 4))
			break;
	};
}

void hdmi_audio_reset() {
	uart_puts("hdmi_audio_reset\n");
	uint32_t mai_ctl = 0;
	hdmi_audio_stop_packet(1);
	mai_ctl = (1 << 0);  //RST
	mai_ctl = (1 << 2);  //UF
	mai_ctl = (1 << 9);  //FLUSH
	*HDMI_MAI_CTL = mai_ctl;
}

void hdmi_audio_startup() {
	uart_puts("hdmi_audio_startup\n");
	uint32_t mai_ctl = 0;
	mai_ctl |= (1 << 0);  //RST
	mai_ctl |= (1 << 1);  //OF
	mai_ctl |= (1 << 2);  //UF
	mai_ctl |= (1 << 15); //DLATE
	mai_ctl |= (1 << 9);  //FLUSH
	*HDMI_MAI_CTL = mai_ctl;
}

void hdmi_audio_prepare() {
	uart_puts("hdmi_audio_prepare\n");

	//SMP todo calc
	*HDMI_MAI_SMP = 0x0DCD21F3;

	//MAI
	uint32_t mai_ctl = 0;
	int ch = 2; //koko
	mai_ctl |= (1 << 3);
	mai_ctl |= (ch << 4);
	mai_ctl |= (1 << 12);
	mai_ctl |= (1 << 13);
	*HDMI_MAI_CTL = mai_ctl;

	*HDMI_MAI_FMT = 0x20900; //ch:2, fs:48000hz

	uint32_t cfg = 0;
	cfg |= (1 << 29); // ZERO DATA SAMPLE_FLAT
	cfg |= (1 << 24); // ZERO DATA INACTIVE CH
	cfg |= (0x0 << 10); // B FRAME IDENT (0x08)
	cfg |= (1 << 1) | (1 << 0); //Left, Right

	uart_debug_puts("audio config=", cfg);

	*HDMI_MAI_THR = 0x08080608; //DREQ

	*HDMI_MAI_CONFIG = (1 << 27) | (1 << 26) | (1 << 1) | (1 << 0);
	*HDMI_MAI_CHANNEL_MAP = 0x8;

	*HDMI_AUDIO_PACKET_CONFIG = cfg;

	int cts_n = 128 * 48000 / 1000; //0x1800
	*HDMI_CRP_CFG = cts_n | (1 << 24); //EXTERNAL CTS EN
	*HDMI_CTS_0 = 0x1220A;
	*HDMI_CTS_1 = 0x1220A;

	//Write Frame
	*HDMI_RAM_PACKET_CONFIG |= (1 << 16);
	hdmi_audio_stop_packet(0);
	*HDMI_RAM_PACKET(0, (9 * 4) + 0) = 0x000A0184;
	*HDMI_RAM_PACKET(0, (9 * 4) + 1) = 0x00000170;
	*HDMI_RAM_PACKET(0, (9 * 4) + 2) = 0x00000000;
	*HDMI_RAM_PACKET(0, (9 * 4) + 3) = 0x00000000;
	*HDMI_RAM_PACKET(0, (9 * 4) + 4) = 0x00000000;
	*HDMI_RAM_PACKET(0, (9 * 4) + 5) = 0x00000000;
	*HDMI_RAM_PACKET(0, (9 * 4) + 6) = 0x00000000;
	*HDMI_RAM_PACKET(0, (9 * 4) + 7) = 0x00000000;
	*HDMI_RAM_PACKET(0, (9 * 4) + 8) = 0x00000000;

	*HDMI_RAM_PACKET(0, (9 * 5) + 0) = 0x000A0184;
	*HDMI_RAM_PACKET(0, (9 * 5) + 1) = 0x00000170;
	*HDMI_RAM_PACKET(0, (9 * 5) + 2) = 0x00000000;
	*HDMI_RAM_PACKET(0, (9 * 5) + 3) = 0x00000000;
	*HDMI_RAM_PACKET(0, (9 * 5) + 4) = 0x00000000;
	*HDMI_RAM_PACKET(0, (9 * 5) + 5) = 0x00000000;
	*HDMI_RAM_PACKET(0, (9 * 5) + 6) = 0x00000000;
	*HDMI_RAM_PACKET(0, (9 * 5) + 7) = 0x00000000;
	*HDMI_RAM_PACKET(0, (9 * 5) + 8) = 0x00000000;
	hdmi_audio_start_packet(0);
}

void hdmi_audio_setup() {
	hdmi_audio_reset();
	hdmi_audio_startup();
	hdmi_audio_prepare();
}
