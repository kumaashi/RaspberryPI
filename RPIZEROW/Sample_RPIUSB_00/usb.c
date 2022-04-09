//---------------------------------------------------
// rpiusb for zero rpi
//---------------------------------------------------
#include "common.h"
#include "hw.h"
#include "uart.h"
#include "usb.h"
#include "heap.h"

#define USB_HCINT_MASK 0x7FF

typedef struct rpiusb_spec_request_t {
	uint8_t bmRequestType; 
	uint8_t bRequest; 
	uint16_t wValue; 
	uint16_t wIndex; 
	uint16_t wLength; 
} __attribute__((__packed__)) rpiusb_spec_request;

void rpiusb_print_reg() {
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

void rpiusb_dump_ctrl_buffer(rpiusb_request *preq) {
	uart_puts("-------------------------------------------------------\n");
	uart_dump((uint32_t)preq->out_buffer, 0x20);
	uart_dump((uint32_t)preq->in_buffer, 0x20);
	uart_puts("-------------------------------------------------------\n");
}

void rpiusb_memset(void *p, uint8_t c, size_t sz) {
	uint8_t *d = (uint8_t *)p;
	while(sz--)
		*d++ = c;
}

void rpiusb_clear_buffer_data(rpiusb_request *preq, uint8_t filldata) {
	uint8_t c = filldata;
	uart_debug_puts("LOG : rpiusb_clear_buffer_data: out_buffer=", (uint32_t)preq->out_buffer);
	uart_debug_puts("LOG : rpiusb_clear_buffer_data: in_buffer=", (uint32_t)preq->in_buffer);
	rpiusb_memset(preq->out_buffer, c, USB_BUF_SIZE);
	rpiusb_memset(preq->in_buffer, c, USB_BUF_SIZE);
}

void rpiusb_device_request(rpiusb_request *preq)
{
	uart_debug_puts("LOG : DREQ addr=", preq->addr);
	rpiusb_spec_request usb_req = {};
	rpiusb_memset(&usb_req, 0, sizeof(usb_req));

	int req = preq->req;
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
		usb_req.wValue = (preq->index << 8) | preq->value;
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
		usb_req.wValue = preq->value;
		usb_req.wIndex = 0;
		usb_req.wLength = 0;
	}

	if(req == USB_DREQ_SET_ADDRESS) {
		uart_puts("LOG : USB_DREQ_SET_ADDRESS\n");
		usb_req.bmRequestType = USB_REQ_OUT;
		usb_req.bRequest = USB_DREQ_SET_ADDRESS;
		usb_req.wValue = preq->value;
		usb_req.wIndex = 0;
		usb_req.wLength = 0;
	}

	if(req == USB_DREQ_SET_FEATURE) {
		uart_puts("LOG : USB_DREQ_SET_FEATURE\n");
		usb_req.bmRequestType = USB_REQ_OUT;
		usb_req.bRequest = USB_DREQ_SET_FEATURE;
		usb_req.wValue = preq->value;
		usb_req.wIndex = 0;
		usb_req.wLength = 0;
	}

	int out_ch = preq->out_ch;
	int in_ch = preq->in_ch;
	{
		uint32_t dev_addr = preq->addr;
		uint32_t eptype = 0;
		uint32_t epnum = 0;
		uint32_t mps = 0x40;
		uint32_t isin = 0;
		uint32_t mc = 1;
		*USB_HCCHAR(out_ch) = (dev_addr << 22) | (mc << 20) | (eptype << 18) | (isin << 15) | (epnum << 11) | mps;
		uart_debug_puts("*USB_HCCHAR(out_ch)=", *USB_HCCHAR(out_ch));
	}

	{
		uint32_t dev_addr = preq->addr;
		uint32_t eptype = 0;
		uint32_t epnum = 0;
		uint32_t mps = 0x40;
		uint32_t isin = 1;
		uint32_t mc = 1;
		*USB_HCCHAR(in_ch) = (dev_addr << 22) | (mc << 20) | (eptype << 18) | (isin << 15) | (epnum << 11) | mps;
		uart_debug_puts("*USB_HCCHAR(in_ch)=", *USB_HCCHAR(in_ch));
	}

	{
		uint8_t pid = 3; //setup
		uint8_t pktcnt = 1;
		uint8_t txlen = sizeof(rpiusb_spec_request);
		*USB_HCTSIZ(out_ch) = (pid << 29) | (pktcnt << 19) | txlen;
	}

	{
		uint8_t pid = 2; //data1
		uint8_t pktcnt = 1;
		uint8_t rxlen = usb_req.wLength;
		*USB_HCTSIZ(in_ch) = (pid << 29) | (pktcnt << 19) | rxlen; 
	}
	*(rpiusb_spec_request *)(&preq->out_buffer[0]) = usb_req;

	//setup buffers
	*USB_HCDMA(out_ch) = (uint32_t)(&preq->out_buffer[0]);
	*USB_HCDMA(out_ch) |= VCADDR_BASE;

	*USB_HCDMA(in_ch) = (uint32_t)(&preq->in_buffer[0]);
	*USB_HCDMA(in_ch) |= VCADDR_BASE;

	//kick
	*USB_HCCHAR(out_ch) |= (1 << 31);
	*USB_HCCHAR(in_ch) |= (1 << 31);
}

void rpiusb_interface_request(rpiusb_request *preq)
{
	uart_debug_puts("LOG : IREQ addr=", preq->addr);
	rpiusb_spec_request usb_req = {};

	//Interface Request.
	usb_req.bmRequestType = 0x01;

	if(preq->req == USB_IREQ_GET_STATUS) {
		uart_puts("LOG : USB_IREQ_GET_STATUS\n");
		usb_req.bmRequestType |= USB_REQ_IN;
		usb_req.bRequest = USB_IREQ_GET_STATUS;
		usb_req.wValue = preq->value;
		usb_req.wIndex = preq->index;
		usb_req.wLength = 2;
	}

	if(preq->req == USB_IREQ_GET_INTERFACE) {
		uart_puts("LOG : USB_IREQ_GET_INTERFACE\n");
		usb_req.bmRequestType |= USB_REQ_IN;
		usb_req.bRequest = USB_IREQ_GET_INTERFACE;
		usb_req.wValue = 0;
		usb_req.wIndex = preq->index;
		usb_req.wLength = 8;
	}

	if(preq->req == USB_IREQ_SET_INTERFACE) {
		uart_puts("LOG : USB_IREQ_SET_INTERFACE\n");
		usb_req.bmRequestType |= USB_REQ_OUT;
		usb_req.bRequest = USB_IREQ_SET_INTERFACE;
		usb_req.wValue = preq->value;
		usb_req.wIndex = preq->index;
		usb_req.wLength = 0;
	}

	if(preq->req == USB_IREQ_SET_FEATURE) {
		uart_puts("LOG : USB_IREQ_SET_FEATURE\n");
		usb_req.bmRequestType |= USB_REQ_OUT;
		usb_req.bRequest = USB_IREQ_SET_FEATURE;
		usb_req.wValue = preq->value;
		usb_req.wIndex = preq->index;
		usb_req.wLength = 0;
	}

	{
		uint32_t dev_addr = preq->addr;
		uint32_t eptype = 0;
		uint32_t epnum = 0;
		uint32_t mps = 0x200;
		uint32_t isin = 0;
		uint32_t mc = 1;
		*USB_HCCHAR(0) = (dev_addr << 22) | (mc << 20) | (eptype << 18) | (isin << 15) | (epnum << 11) | mps;
	}

	{
		uint32_t dev_addr = preq->addr;
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
		uint8_t txlen = sizeof(rpiusb_spec_request);
		*USB_HCTSIZ(0) = (pid << 29) | (pktcnt << 19) | txlen;
	}

	{
		//data1
		uint8_t pid = 2;
		uint8_t pktcnt = 1;
		uint8_t rxlen = usb_req.wLength;
		*USB_HCTSIZ(1) = (pid << 29) | (pktcnt << 19) | rxlen; 
	}
	*(rpiusb_spec_request *)(&preq->out_buffer[0]) = usb_req;

	//setup buffers
	*USB_HCDMA(0) = (uint32_t)(&preq->out_buffer[0]);
	*USB_HCDMA(0) |= VCADDR_BASE;

	*USB_HCDMA(1) = (uint32_t)(&preq->in_buffer[0]);
	*USB_HCDMA(1) |= VCADDR_BASE;

	InvalidateData();
	*USB_HCCHAR(0) |= (1 << 31);
	*USB_HCCHAR(1) |= (1 << 31);
}

void rpiusb_trans_intr(rpiusb_trans_data *pdata) {
	int ch = pdata->ch;
	{
		uint32_t mps = 0x40;
		uint32_t hcchar = 0;
		hcchar |= pdata->dev_addr << 22;

		//INTR
		hcchar |= 3 << 18;
		hcchar |= (pdata->isin ? 1 : 0) << 15;
		hcchar |= pdata->epnum << 11;
		hcchar |= mps;
		*USB_HCCHAR(ch) = hcchar;
	}

	{
		uint8_t pid = (pdata->isdata1) ? 2 : 0;
		uint8_t pktcnt = 1;
		uint8_t txlen = pdata->len;
		*USB_HCTSIZ(ch) = (pid << 29) | (pktcnt << 19) | txlen;
		uart_debug_puts("rpiusb_trans_intr : *USB_HCTSIZ(ch)=", *USB_HCTSIZ(ch));
	}

	//setup buffers
	*USB_HCDMA(ch) = (uint32_t)pdata->buffer;
	*USB_HCDMA(ch) |= VCADDR_BASE;

	//enable dma
	*USB_HCCHAR(ch) |= (1 << 31);
}

void rpiusb_core_reset() {
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

void rpiusb_hc_prt_poweron_reset() {
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

void rpiusb_clear_global_int() {
	*USB_GINTSTS = *USB_GINTSTS;
}

void rpiusb_hc_clear_int(int ch) {
	*USB_HCINT(ch) |= USB_HCINT_MASK;
}

uint32_t rpiusb_hc_get_stall(int port) {
	return USB_HCINT_STALL & *USB_HCINT(port);
}

uint32_t rpiusb_hc_get_nak(int port) {
	return USB_HCINT_NAK & *USB_HCINT(port);
}

uint32_t rpiusb_hc_get_ack(int port) {
	return USB_HCINT_ACK & *USB_HCINT(port);
}

uint32_t rpiusb_hc_get_ch_halt(int port) {
	return USB_HCINT_CHHLTD &  *USB_HCINT(port);
}

uint32_t rpiusb_hc_get_xfer_compl(int port) {
	return USB_HCINT_XFERCOMPL &  *USB_HCINT(port);
}

