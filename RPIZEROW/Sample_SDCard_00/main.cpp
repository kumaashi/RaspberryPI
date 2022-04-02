// For Raspberry Pi Zero W V3D test
// 2022 yasai kumaashi (gyaboyan@gmail.com)

extern "C" {
#include "common.h"
#include "uart.h"
#include "hw.h"
#include "led.h"
#include "mailbox.h"
#include "dma.h"
#include "irq.h"
} //EXTERN C

#define WIDTH             640
#define HEIGHT            480
#define BUFNUM            2
#define TILE_SIZE         32

void emmc_debug_puts() {
	uart_puts("----------------------------------------------------------------\n");
	uart_debug_puts("EMMC_ARG2                =", *EMMC_ARG2           );
	uart_debug_puts("EMMC_BLKSIZECNT          =", *EMMC_BLKSIZECNT     );  
	uart_debug_puts("EMMC_ARG1                =", *EMMC_ARG1           );  
	uart_debug_puts("EMMC_CMDTM               =", *EMMC_CMDTM          );  
	uart_debug_puts("EMMC_RESP0               =", *EMMC_RESP0          );  
	uart_debug_puts("EMMC_RESP1               =", *EMMC_RESP1          );  
	uart_debug_puts("EMMC_RESP2               =", *EMMC_RESP2          );  
	uart_debug_puts("EMMC_RESP3               =", *EMMC_RESP3          );  
	uart_debug_puts("EMMC_DATA                =", *EMMC_DATA           );  
	uart_debug_puts("EMMC_STATUS              =", *EMMC_STATUS         );  
	uart_debug_puts("EMMC_CONTROL0            =", *EMMC_CONTROL0       );  
	uart_debug_puts("EMMC_CONTROL1            =", *EMMC_CONTROL1       );  
	uart_debug_puts("EMMC_INTERRUPT           =", *EMMC_INTERRUPT      );  
	uart_debug_puts("EMMC_IRPT_MASK           =", *EMMC_IRPT_MASK      );  
	uart_debug_puts("EMMC_IRPT_EN             =", *EMMC_IRPT_EN        );  
	uart_debug_puts("EMMC_CONTROL2            =", *EMMC_CONTROL2       );  
	uart_debug_puts("EMMC_FORCE_IRPT          =", *EMMC_FORCE_IRPT     );  
	uart_debug_puts("EMMC_BOOT_TIMEOUT        =", *EMMC_BOOT_TIMEOUT   );  
	uart_debug_puts("EMMC_DBG_SEL             =", *EMMC_DBG_SEL        );  
	uart_puts("----------------------------------------------------------------\n");
	//uart_debug_puts("EMMC_EXRDFIFO_CFG        =", *EMMC_EXRDFIFO_CFG   );  
	//uart_debug_puts("EMMC_EXRDFIFO_EN         =", *EMMC_EXRDFIFO_EN    );  
	//uart_debug_puts("EMMC_TUNE_STEP           =", *EMMC_TUNE_STEP      );  
	//uart_debug_puts("EMMC_TUNE_STEPS_STD      =", *EMMC_TUNE_STEPS_STD );  
	//uart_debug_puts("EMMC_TUNE_STEPS_DDR      =", *EMMC_TUNE_STEPS_DDR );  
	//uart_debug_puts("EMMC_SPI_INT_SPT         =", *EMMC_SPI_INT_SPT    );  
	//uart_debug_puts("EMMC_SLOTISR_VER         =", *EMMC_SLOTISR_VER    );  
}

void emmc_issue_cmd(int index, int resptype, int card_to_host, int isautocmd, int isblkcnten)
{
	uint32_t cmd = 0;
	cmd |= (index << 24);
	if(index == 24 || index == 25 || index == 16 || index == 17)
		cmd |= (1 << 21);
	if(index == 25 || index == 17)
		cmd |= (1 << 5);
	cmd |= (1 << 19);
	cmd |= (resptype << 16);
	cmd |= (card_to_host << 4);
	cmd |= (isautocmd << 2);
	cmd |= (isblkcnten << 1);
	*EMMC_CMDTM = cmd;
}

int maincpp(void) {
	led_init();
	uart_init();
	mailbox_set_domain_state(0, 1);
	mailbox_set_power_state(0, 1);
	mailbox_set_clock_rate(MAILBOX_CLOCK_ID_EMMC, 1);

	mailbox_fb_init(WIDTH, HEIGHT, BUFNUM);
	mailbox_fb *fb = mailbox_fb_get();

	int count = 0;
	while(1) {
		led_set(count & 1);
		//CMD0
		emmc_issue_cmd(0, 0, 0, 0, 0);
		SLEEP(0x10000);
		emmc_debug_puts();

		//CMD55
		emmc_issue_cmd(55, 1, 0, 0, 0);
		SLEEP(0x10000);
		emmc_debug_puts();

		//CMD41
		emmc_issue_cmd(41, 1, 0, 0, 0);
		SLEEP(0x10000);
		emmc_debug_puts();

		count++;
		SLEEP(0x100000);
	}
	return(0);
}

extern "C" {
	int notmain() {
		maincpp();
		return 0;
	}
}
