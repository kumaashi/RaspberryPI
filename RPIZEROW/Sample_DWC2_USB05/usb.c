#include "common.h"
#include "hw.h"
#include "uart.h"
#include "heap.h"

//usb_20.pdf
#define USB_REQ_OUT                (0x00)
#define USB_REQ_IN                 (0x80)
#define USB_DREQ_GET_STATUS        (0x00) //breq : 1000 0000b
#define USB_DREQ_CLEAR_FEATURE     (0x01) //breq : 0000 0000b
#define USB_DREQ_SET_FEATURE       (0x03) //breq : 0000 0000b
#define USB_DREQ_SET_ADDRESS       (0x05) //breq : 0000 0000b
#define USB_DREQ_GET_DESCRIPTOR    (0x06) //breq : 1000 0000b
#define USB_DREQ_SET_DESCRIPTOR    (0x07) //breq : 0000 0000b
#define USB_DREQ_GET_CONFIGURATION (0x08) //breq : 1000 0000b
#define USB_DREQ_SET_CONFIGURATION (0x09) //breq : 0000 0000b
#define USB_IREQ_GET_STATUS        (0x00) //breq : 1000 0001b 
#define USB_IREQ_CLEAR_FEATURE     (0x01) //breq : 0000 0001b
#define USB_IREQ_SET_FEATURE       (0x03) //breq : 0000 0001b
#define USB_IREQ_GET_INTERFACE     (0x0A) //breq : 1000 0001b
#define USB_IREQ_SET_INTERFACE     (0x0B) //breq : 0000 0001b

typedef struct usb_request_t {
	//1 Bitmap Characteristics of request:
	uint8_t bmRequestType; 

	//1 Value Specific request (refer to Table 9-3)
	uint8_t bRequest; 

	//2 Value Word-sized field that varies according to
	uint16_t wValue; 
	uint16_t wIndex; 
	uint16_t wLength; 
} __attribute__((__packed__)) usb_request;
_Static_assert(sizeof(usb_request) == 8);

//Buffers
#define USB_BUF_SIZE 512
static uint8_t *usb_out_buffer;
static uint8_t *usb_in_buffer;

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
	dwc2_memset(usb_out_buffer, c, USB_BUF_SIZE);
	dwc2_memset(usb_in_buffer, c, USB_BUF_SIZE);
}

void dwc2_dump_ctrl_buffer() {
	uart_puts("-------------------------------------------------------\n");
	uart_dump((uint32_t)usb_out_buffer, 0x20);
	uart_dump((uint32_t)usb_in_buffer, 0x20);
	uart_puts("-------------------------------------------------------\n");
}

void dwc2_print_reg() {
	uart_puts("-------------------------------------------------------\n");
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
	uart_debug_puts("USB_PCGCTL        :", *USB_PCGCTL); 
	uart_debug_puts("USB_GUID          :", *USB_GUID); 
	uart_debug_puts("USB_GSNPSID       :", *USB_GSNPSID); 
	uart_debug_puts("USB_EX_MDIO_VBUS  :", *USB_EX_MDIO_VBUS); 
	uart_puts("-------------------------------------------------------\n");
}

uint32_t dwc2_usb_get_stall(int port) {
	return (1 << 3) & *USB_HCINT(port);
}

uint32_t dwc2_usb_get_nak(int port) {
	return (1 << 4) & *USB_HCINT(port);
}

uint32_t dwc2_usb_get_ack(int port) {
	return (1 << 5) & *USB_HCINT(port);
}

uint32_t dwc2_usb_get_xfer_ok(int port) {
	return *USB_HCINT(port) == 0x23;
}

void
dwc2_usb_intr(
		int ch,
		void *buffer,
		uint8_t len,
		int dev_addr,
		int epnum,
		int isin)
{
	{
		uint32_t mps = 0x40;
		uint32_t hcchar = 0;
		hcchar |= dev_addr << 22;
		//INTR
		hcchar |= 3 << 18;
		hcchar |= (isin ? 1 : 0) << 15;
		hcchar |= epnum << 11;
		hcchar |= mps;
		*USB_HCCHAR(ch) = hcchar;
	}

	{
		//data0
		uint8_t pid = 2; 
		uint8_t pktcnt = 1;
		uint8_t txlen = len;
		*USB_HCTSIZ(ch) = (pid << 29) | (pktcnt << 19) | txlen;
	}

	//setup buffers
	*USB_HCDMA(ch) = (uint32_t)buffer;
	*USB_HCDMA(ch) |= VCADDR_BASE;

	//enable dma
	InvalidateData();
	*USB_HCCHAR(ch) |= (1 << 31);
}



	void
dwc2_device_request(
		int req,
		int addr,
		uint16_t value,
		uint16_t index,
		int size)
{
	uart_debug_puts("LOG : DREQ addr=", addr);
	usb_request usb_req = {};

	if(req == USB_DREQ_GET_STATUS) {
		uart_puts("LOG : USB_DREQ_GET_STATUS\n");
		usb_req.bmRequestType = USB_REQ_IN;
		usb_req.bRequest = USB_DREQ_GET_STATUS;
		usb_req.wValue = 0;
		usb_req.wIndex = 0;
		usb_req.wLength = 2;
	}

	if(req == USB_DREQ_GET_DESCRIPTOR) {
		uart_puts("LOG : USB_DREQ_GET_DESCRIPTOR\n");
		usb_req.bmRequestType = USB_REQ_IN;
		usb_req.bRequest = USB_DREQ_GET_DESCRIPTOR;
		usb_req.wValue = (index << 8) | value;
		usb_req.wIndex = 0;
		usb_req.wLength = 12;
	}

	if(req == USB_DREQ_GET_CONFIGURATION) {
		uart_puts("LOG : USB_DREQ_GET_CONFIGURATION\n");
		usb_req.bmRequestType = USB_REQ_IN;
		usb_req.bRequest = USB_DREQ_GET_CONFIGURATION;
		usb_req.wValue = 0;
		usb_req.wIndex = 0;
		usb_req.wLength = 1;
	}

	if(req == USB_DREQ_SET_CONFIGURATION) {
		uart_puts("LOG : USB_DREQ_SET_CONFIGURATION\n");
		usb_req.bmRequestType = USB_REQ_OUT;
		usb_req.bRequest = USB_DREQ_SET_CONFIGURATION;
		usb_req.wValue = value;
		usb_req.wIndex = 0;
		usb_req.wLength = 0;
	}

	if(req == USB_DREQ_SET_ADDRESS) {
		uart_puts("LOG : USB_DREQ_SET_ADDRESS\n");
		usb_req.bmRequestType = USB_REQ_OUT;
		usb_req.bRequest = USB_DREQ_SET_ADDRESS;
		usb_req.wValue = value;
		usb_req.wIndex = 0;
		usb_req.wLength = 0;
	}

	if(req == USB_DREQ_SET_FEATURE) {
		uart_puts("LOG : USB_DREQ_SET_FEATURE\n");
		usb_req.bmRequestType = USB_REQ_OUT;
		usb_req.bRequest = USB_DREQ_SET_FEATURE;
		usb_req.wValue = value;
		usb_req.wIndex = 0;
		usb_req.wLength = 0;
	}

	{
		uint32_t dev_addr = addr;
		uint32_t eptype = 0;
		uint32_t epnum = 0;
		uint32_t mps = 0x40;
		uint32_t isin = 0;
		uint32_t mc = 1;
		*USB_HCCHAR(0) = (dev_addr << 22) | (mc << 20) | (eptype << 18) | (isin << 15) | (epnum << 11) | mps;
	}

	{
		uint32_t dev_addr = addr;
		uint32_t eptype = 0;
		uint32_t epnum = 0;
		uint32_t mps = 0x40;
		uint32_t isin = 1;
		uint32_t mc = 1;
		*USB_HCCHAR(1) = (dev_addr << 22) | (mc << 20) | (eptype << 18) | (isin << 15) | (epnum << 11) | mps;
	}

	{
		uint8_t pid = 3; //setup
		uint8_t pktcnt = 1;
		uint8_t txlen = sizeof(usb_request);
		*USB_HCTSIZ(0) = (pid << 29) | (pktcnt << 19) | txlen;
	}

	{
		uint8_t pid = 2; //data1
		uint8_t pktcnt = 1;
		uint8_t rxlen = usb_req.wLength;
		*USB_HCTSIZ(1) = (pid << 29) | (pktcnt << 19) | rxlen; 
	}
	*(usb_request *)(&usb_out_buffer[0]) = usb_req;

	//setup buffers
	*USB_HCDMA(0) = (uint32_t)(&usb_out_buffer[0]);
	*USB_HCDMA(0) |= VCADDR_BASE;

	*USB_HCDMA(1) = (uint32_t)(&usb_in_buffer[0]);
	*USB_HCDMA(1) |= VCADDR_BASE;

	InvalidateData();
	*USB_HCCHAR(0) |= (1 << 31);
	*USB_HCCHAR(1) |= (1 << 31);
}

void dwc2_interface_request(int req, int addr, uint16_t value, uint16_t index)
{
	uart_debug_puts("LOG : IREQ addr=", addr);
	usb_request usb_req = {};

	//Interface Request.
	usb_req.bmRequestType = 0x01;

	if(req == USB_IREQ_GET_STATUS) {
		uart_puts("LOG : USB_IREQ_GET_STATUS\n");
		usb_req.bmRequestType |= USB_REQ_IN;
		usb_req.bRequest = USB_IREQ_GET_STATUS;
		usb_req.wValue = value;
		usb_req.wIndex = index;
		usb_req.wLength = 2;
	}

	if(req == USB_IREQ_GET_INTERFACE) {
		uart_puts("LOG : USB_IREQ_GET_INTERFACE\n");
		usb_req.bmRequestType |= USB_REQ_IN;
		usb_req.bRequest = USB_IREQ_GET_INTERFACE;
		usb_req.wValue = 0;
		usb_req.wIndex = index;
		usb_req.wLength = 8;
	}

	if(req == USB_IREQ_SET_INTERFACE) {
		uart_puts("LOG : USB_IREQ_SET_INTERFACE\n");
		usb_req.bmRequestType |= USB_REQ_OUT;
		usb_req.bRequest = USB_IREQ_SET_INTERFACE;
		usb_req.wValue = value;
		usb_req.wIndex = index;
		usb_req.wLength = 0;
	}

	if(req == USB_IREQ_SET_FEATURE) {
		uart_puts("LOG : USB_IREQ_SET_FEATURE\n");
		usb_req.bmRequestType |= USB_REQ_OUT;
		usb_req.bRequest = USB_IREQ_SET_FEATURE;
		usb_req.wValue = value;
		usb_req.wIndex = index;
		usb_req.wLength = 0;
	}

	{
		uint32_t dev_addr = addr;
		uint32_t eptype = 0;
		uint32_t epnum = 0;
		uint32_t mps = 0x200;
		uint32_t isin = 0;
		uint32_t mc = 1;
		*USB_HCCHAR(0) = (dev_addr << 22) | (mc << 20) | (eptype << 18) | (isin << 15) | (epnum << 11) | mps;
	}

	{
		uint32_t dev_addr = addr;
		uint32_t eptype = 0;
		uint32_t epnum = 0;
		uint32_t mps = 0x200;
		uint32_t isin = 1;
		uint32_t mc = 1;
		*USB_HCCHAR(1) = (dev_addr << 22) | (mc << 20) | (eptype << 18) | (isin << 15) | (epnum << 11) | mps;
	}

	{
		//setup
		uint8_t pid = 3;
		uint8_t pktcnt = 1;
		uint8_t txlen = sizeof(usb_request);
		*USB_HCTSIZ(0) = (pid << 29) | (pktcnt << 19) | txlen;
	}

	{
		//data1
		uint8_t pid = 2;
		uint8_t pktcnt = 1;
		uint8_t rxlen = usb_req.wLength;
		*USB_HCTSIZ(1) = (pid << 29) | (pktcnt << 19) | rxlen; 
	}
	*(usb_request *)(&usb_out_buffer[0]) = usb_req;

	//setup buffers
	*USB_HCDMA(0) = (uint32_t)(&usb_out_buffer[0]);
	*USB_HCDMA(0) |= VCADDR_BASE;

	*USB_HCDMA(1) = (uint32_t)(&usb_in_buffer[0]);
	*USB_HCDMA(1) |= VCADDR_BASE;

	InvalidateData();
	*USB_HCCHAR(0) |= (1 << 31);
	*USB_HCCHAR(1) |= (1 << 31);
}

void dwc2_core_reset() {
	usb_out_buffer = (uint8_t *)heap_get();
	usb_in_buffer = (uint8_t *)heap_get();

	//Reset Clock
	*USB_PCGCTL = 0;
	SLEEP(0x100000);

	//RESET DWC2
	*USB_GRSTCTL = (1 << 0);
	SLEEP(0x400000);

	//ENABLE DMA
	*USB_GAHBCFG |= (1 << 5);

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

void dwc2_host_clear_global_int() {
	uint32_t reg = *USB_GINTSTS;
	*USB_GINTSTS = reg;
}

void dwc2_host_clear_int() {
	dwc2_host_clear_global_int();
	*USB_GINTSTS = *USB_GINTSTS;
	*USB_HCINT(0) |= 0x7FF;
	*USB_HCINT(1) |= 0x7FF;
	*USB_HCINT(2) |= 0x7FF;
}
