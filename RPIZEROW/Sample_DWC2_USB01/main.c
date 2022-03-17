// Raspberry Pi ARMv6 DWC2 tentative driver
// 2022 yasai kumaashi (gyaboyan@gmail.com)
// ref:https://github.com/dwelch67/raspberrypi-zero/
#include "common.h"
#include "uart.h"
#include "hw.h"
#include "led.h"

//Buffers
static uint8_t usb_out_buffer[1024] __attribute__ ((aligned (16384))) ;
static uint8_t usb_in_buffer[1024] __attribute__ ((aligned (16384))) ;
static uint8_t usb_2_buffer[1024] __attribute__ ((aligned (16384))) ;

void dwc2_memcpy(void *dest, void *src, size_t n) {
	char *src_char = (char *)src;
	char *dest_char = (char *)dest;
	for (size_t i=0; i<n; i++) {
		dest_char[i] = src_char[i];
	}
}

void dwc2_memset(void *p, uint8_t c, size_t sz) {
	uint8_t *d = (uint8_t *)p;
	while(sz--)
		*d++ = c;
}

void dwc2_clear_buffer_data(uint8_t filldata) {
	uint8_t c = filldata;
	dwc2_memset(usb_out_buffer, c, sizeof(usb_out_buffer));
	dwc2_memset(usb_in_buffer, c, sizeof(usb_in_buffer));
	dwc2_memset(usb_2_buffer, c, sizeof(usb_2_buffer));
}

void print_reg() {
	uart_puts("-----------------------------------------------------------\n");
	uart_debug_puts("INTR_IRQ_BASIC_PENDING       :", *INTR_IRQ_BASIC_PENDING   ); 
	uart_debug_puts("IRQ_ENABLE1       :", *IRQ_ENABLE1   ); 
	uart_debug_puts("USB_GAHBCFG       :", *USB_GAHBCFG    ); 
	uart_debug_puts("USB_GUSBCFG       :", *USB_GUSBCFG    ); 
	uart_debug_puts("USB_GRSTCTL       :", *USB_GRSTCTL    ); 
	uart_debug_puts("USB_GINTSTS       :", *USB_GINTSTS    ); 
	uart_debug_puts("USB_GINTMSK       :", *USB_GINTMSK    ); 
	uart_debug_puts("USB_GNPTXFSIZ     :", *USB_GNPTXFSIZ);
	uart_debug_puts("USB_HAINT         :", *USB_HAINT    ); 
	uart_debug_puts("USB_HPRT0         :", *USB_HPRT0    ); 
	uart_debug_puts("USB_HCFG          :", *USB_HCFG    ); 
	uart_debug_puts("USB_HFNUM         :", *USB_HFNUM    ); 
	uart_debug_puts("USB_HFIR          :", *USB_HFIR); 
	uart_debug_puts("USB_HCCHAR(0)     :", *USB_HCCHAR(0));
	uart_debug_puts("USB_HCCHAR(1)     :", *USB_HCCHAR(1));
	uart_debug_puts("USB_HCCHAR(2)     :", *USB_HCCHAR(2));
	uart_debug_puts("USB_HCDMA(0)      :", *USB_HCDMA(0));
	uart_debug_puts("USB_HCDMA(1)      :", *USB_HCDMA(1));
	uart_debug_puts("USB_HCDMA(2)      :", *USB_HCDMA(2));
	uart_debug_puts("USB_HCINT(0)      :", *USB_HCINT(0)); 
	uart_debug_puts("USB_HCINT(1)      :", *USB_HCINT(1)); 
	uart_debug_puts("USB_HCINT(2)      :", *USB_HCINT(2)); 
	uart_debug_puts("USB_HCINTMSK(0)   :", *USB_HCINTMSK(0)); 
	uart_debug_puts("USB_HCINTMSK(1)   :", *USB_HCINTMSK(1)); 
	uart_debug_puts("USB_HCINTMSK(2)   :", *USB_HCINTMSK(2)); 
	uart_debug_puts("USB_HCTSIZ(0)     :", *USB_HCTSIZ(0)); 
	uart_debug_puts("USB_HCTSIZ(1)     :", *USB_HCTSIZ(1)); 
	uart_debug_puts("USB_HCTSIZ(2)     :", *USB_HCTSIZ(2)); 
	uart_debug_puts("usb_out_buffer    :", (uint32_t)usb_out_buffer);
	uart_debug_puts("usb_in_buffer     :", (uint32_t)usb_in_buffer);
	uart_debug_puts("USB_PCGCTL        :", *USB_PCGCTL); 
	uart_debug_puts("USB_GUID          :", *USB_GUID); 
	uart_debug_puts("USB_GSNPSID       :", *USB_GSNPSID); 
	uart_debug_puts("USB_EX_MDIO_VBUS  :", *USB_EX_MDIO_VBUS); 

	uart_puts("\nOUT : ");
	for(int i = 0 ; i < 16; i++) {
		uart_putc_hex(usb_out_buffer[i]);
		uart_putc(' ');
	}
	uart_puts("\nIN  : ");
	for(int i = 0 ; i < 16; i++) {
		uart_putc_hex(usb_in_buffer[i]);
		uart_putc(' ');
	}
	uart_puts("\nCH2 : ");
	for(int i = 0 ; i < 16; i++) {
		uart_putc_hex(usb_2_buffer[i]);
		uart_putc(' ');
	}
	uart_puts("\n");
	uart_puts("-----------------------------------------------------------\n");
}

void handle_hang() {
	led_init();
	uart_init();
	while(1) {
		uart_puts("HANG!!!!!!!!!\n");
		SLEEP(0x10000);
	}
}

void dwc2_send_packet_get_descriptor(int index, int type)
{
	{
		uint32_t dev_addr = 0;
		uint32_t eptype = 0; //3:intr
		uint32_t epnum = 0;
		uint32_t mps = 0x40;
		uint32_t isin = 0;
		uint32_t mc = 1;
		*USB_HCCHAR(0) = (dev_addr << 22) | (mc << 20) | (eptype << 18) | (isin << 15) | (epnum << 11) | mps;
	}

	{
		uint32_t dev_addr = 0;
		uint32_t eptype = 0; //3:intr
		uint32_t epnum = 0;
		uint32_t mps = 0x40;
		uint32_t isin = 1;
		uint32_t mc = 1;
		*USB_HCCHAR(1) = (dev_addr << 22) | (mc << 20) | (eptype << 18) | (isin << 15) | (epnum << 11) | mps;
	}

	//type
	usb_out_buffer[0] = 0x80; //device -> host

	//request
	usb_out_buffer[1] = 0x06; //get_descriptor

	//value le
	usb_out_buffer[2] = index; //index
	usb_out_buffer[3] = type; //type

	//index le
	usb_out_buffer[4] = 0x00;
	usb_out_buffer[5] = 0x00;

	//length le
	usb_out_buffer[6] = 0x40;
	usb_out_buffer[7] = 0x00;

	//setup buffers
	*USB_HCDMA(0) = (uint32_t)(&usb_out_buffer[0]);
	*USB_HCDMA(0) |= 0xC0000000; //busaddr

	*USB_HCDMA(1) = (uint32_t)(&usb_in_buffer[0]);
	*USB_HCDMA(1) |= 0xC0000000; //busaddr

	{
		uint8_t pid = 3; //setup
		uint8_t pktcnt = 1;
		uint8_t txlen = 0x8;
		*USB_HCTSIZ(0) = (pid << 29) | (pktcnt << 19) | txlen;
	}

	{
		uint8_t pid = 2; //data1
		uint8_t pktcnt = 1;
		uint8_t rxlen = 0x40;
		*USB_HCTSIZ(1) = (pid << 29) | (pktcnt << 19) | rxlen; 
	}

	//enable dma
	uint32_t hcchar0 = *USB_HCCHAR(0);
	uint32_t hcchar1 = *USB_HCCHAR(1);

	hcchar0 |= (1 << 31);
	hcchar0 &= ~(1 << 30);

	hcchar1 |= (1 << 31);
	hcchar1 &= ~(1 << 30);

	*USB_HCCHAR(0) = hcchar0;
	*USB_HCCHAR(1) = hcchar1;
}

void dwc2_core_reset() {
	//POWER ON
	*USB_EX_MDIO_VBUS = 0xE;

	//Reset Clock
	*USB_PCGCTL = 0;
	SLEEP(0x100000);

	//RESET DWC2
	*USB_GRSTCTL = (1 << 0);// | (16 << 6);
	SLEEP(0x400000);

	/*	*/
	//cfg
	*USB_GAHBCFG |= (1 << 5); //ENABLE DMA

	//Enable Global Intr
	*USB_GAHBCFG |= (1 << 0);
}

void dwc2_hprt_poweron_reset() {
	//set FS
	*USB_HPRT0 |= (1 << 17); //FS
	*USB_HPRT0 |= (1 << 3);

	// USB Host power on
	*USB_HPRT0 |= (1 << 12);
	SLEEP(WAIT_CNT * 10);

	//RESET
	*USB_HPRT0 |= (1 << 8);
	SLEEP(WAIT_CNT * 10);

	//RESET deassert
	*USB_HPRT0 &= ~(1 << 8);
	SLEEP(WAIT_CNT * 10);

}

void intr_handler() {
	uart_puts("call intr_handler\n");
}


#define USB_HPRT0_HCINT    (1 << 25)
#define USB_HPRT0_PRTINT   (1 << 24)
#define USB_HPRT0_INCOMPLP (1 << 21)

void dwc2_host_clear_int() {
	uint32_t reg = *USB_GINTSTS;
	*USB_GINTSTS = reg;
}

void dwc2_host_enable_gintmsk() {
	*USB_GINTMSK = USB_HPRT0_HCINT | USB_HPRT0_PRTINT | USB_HPRT0_INCOMPLP;
	*USB_HAINTMSK = 3;
	*USB_HCINTMSK(0) = 0x7FF;
	*USB_HCINTMSK(1) = 0x7FF;
	*USB_HCINTMSK(2) = 0x7FF;
}

int notmain(void) {
	led_init();
	uart_init();

	//reset core
	dwc2_core_reset();

	//reset port
	dwc2_hprt_poweron_reset();

	int count = 0;
	while(1)
	{
		count++;
		uart_puts("###### LOG : BLINK LED\n");
		led_set(count & 1);
		
		uart_puts("###### LOG : CLEAR BUFFER\n");
		dwc2_clear_buffer_data(0x5A);

		uart_puts("###### LOG : OUT GET_DESCRIPTOR\n");
		dwc2_send_packet_get_descriptor(0, 1);
		SLEEP(0x400000);

		print_reg();
		*USB_GINTSTS = *USB_GINTSTS;
		*USB_HCINT(0) |= 0x7FF;//(1 << 4);
		*USB_HCINT(1) |= 0x7FF;//(1 << 4);
	}

	return(0);
}

