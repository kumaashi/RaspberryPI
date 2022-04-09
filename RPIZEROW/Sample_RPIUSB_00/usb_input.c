#include "common.h"
#include "uart.h"
#include "hw.h"
#include "mailbox.h"
#include "usb.h"
#include "usb_input.h"
#include "heap.h"

#define CH_CTRL_OUT   0
#define CH_CTRL_IN    1
#define CH   2
#define ADDR 1

//todo contexitarization
static rpiusb_request ctrl_req;
static rpiusb_trans_data trans_intr;

int usb_input_init(void) {
	ctrl_req.out_buffer = (uint8_t *)heap4k_get();
	ctrl_req.in_buffer = (uint8_t *)heap4k_get();
	ctrl_req.out_ch = CH_CTRL_OUT;
	ctrl_req.in_ch = CH_CTRL_IN;

	trans_intr.ch = CH;
	trans_intr.buffer = (uint8_t *)heap4k_get();

	//https://github.com/raspberrypi/linux/blob/aeaa2460db088fb2c97ae56dec6d7d0058c68294/drivers/soc/bcm/raspberrypi-power.c#L20
	uart_puts("START : POWER_DOMAIN_USB ON\n");
	const int POWER_DOMAIN_USB = 3;
	mailbox_set_power_state(POWER_DOMAIN_USB, 1);
	uart_puts("DONE : POWER_DOMAIN_USB ON\n");

	//reset core
	uart_puts("START : rpiusb_core_reset\n");
	rpiusb_core_reset();
	uart_puts("DONE : rpiusb_core_reset\n");

	//reset port
	uart_puts("START : rpiusb_hc_prt_poweron_reset\n");
	rpiusb_hc_prt_poweron_reset();
	uart_puts("DONE : rpiusb_hc_prt_poweron_reset\n");



	//----------------------------------------------------
	//Should be preparing to obtain to usb_input descriptor interface and ep witu using lsusb -D
	//D - GET_DESCRIPTOR
	uart_puts("START : USB_DREQ_GET_DESCRIPTOR\n");
	ctrl_req.req = USB_DREQ_GET_DESCRIPTOR;
	ctrl_req.addr = 0;
	ctrl_req.value = 0;
	ctrl_req.index = 1; //spec
	ctrl_req.size = 0;
	rpiusb_hc_clear_int(CH_CTRL_OUT);
	rpiusb_hc_clear_int(CH_CTRL_IN);
	rpiusb_clear_buffer_data(&ctrl_req, 0xCC);
	rpiusb_device_request(&ctrl_req);
	SLEEP(WAIT_CNT);
	rpiusb_print_reg();
	rpiusb_dump_ctrl_buffer(&ctrl_req);
	uart_puts("DONE : USB_DREQ_GET_DESCRIPTOR\n");

	//----------------------------------------------------
	uart_puts("START : SET_ADDRESS\n");
	//D - SET_ADDRESS
	ctrl_req.req = USB_DREQ_SET_ADDRESS;
	ctrl_req.addr = 0;
	ctrl_req.value = ADDR;
	ctrl_req.index = 0;
	ctrl_req.size = 0;
	rpiusb_clear_buffer_data(&ctrl_req, 0xCC);
	rpiusb_device_request(&ctrl_req);
	SLEEP(WAIT_CNT);
	rpiusb_print_reg();
	rpiusb_dump_ctrl_buffer(&ctrl_req);
	uart_puts("DONE : SET_ADDRESS\n");
	
	//
	ctrl_req.addr = ADDR;


	//----------------------------------------------------
	//D - SET_CONFIGURATION
	int config = 1;
	ctrl_req.req = USB_DREQ_SET_CONFIGURATION;
	ctrl_req.value = config;
	ctrl_req.index = 0; //spec
	ctrl_req.size = 0;
	rpiusb_hc_clear_int(CH_CTRL_OUT);
	rpiusb_hc_clear_int(CH_CTRL_IN);
	rpiusb_clear_buffer_data(&ctrl_req, 0xCC);
	rpiusb_device_request(&ctrl_req);
	SLEEP(WAIT_CNT);
	rpiusb_print_reg();
	rpiusb_dump_ctrl_buffer(&ctrl_req);
	return 0;
}

void usb_input_update() {
	/*
	if(is_init == 0) {
		usb_input_buffer[0] = 0x05;
		usb_input_buffer[1] = 0x20;
		usb_input_buffer[2] = 0x00;
		usb_input_buffer[3] = 0x01;
		usb_input_buffer[4] = 0x00;
		rpiusb_usb_intr(CH, (void *)usb_input_buffer, 0x5, ADDR, epindex, 0);
		is_init = 1;
	} else {
	}
	*/

	rpiusb_hc_clear_int(CH);
	trans_intr.ch = CH;
	trans_intr.len = 0x20;
	trans_intr.dev_addr = ADDR;
	trans_intr.epnum = 1;
	trans_intr.isin = 1;

	rpiusb_trans_intr(&trans_intr);
	while(rpiusb_hc_get_ch_halt(CH) == 0)
		SLEEP(2);

	if(!rpiusb_hc_get_nak(CH) || !rpiusb_hc_get_stall(CH))
		trans_intr.isdata1 = (trans_intr.isdata1) ? 0 : 1;

	uint32_t status = *USB_HCINT(CH);
	if(status & USB_HCINT_DATATGLERR) {
		uart_puts("USB_HCINT_DATATGLERR\n");
		rpiusb_print_reg();
		trans_intr.isdata1 = (trans_intr.isdata1) ? 0 : 1;
		//trans_intr.isdata1 = 0;
	}
	if(status & USB_HCINT_BBLERR)
		uart_puts("USB_HCINT_BBLERR\n");
	if(status & USB_HCINT_XACTERR)
		uart_puts("USB_HCINT_XACTERR\n");
	
	//debug
	static int count = 0;
	count++;
	if((count % 60) == 0) {
		uart_debug_puts("hcint2=", *USB_HCINT(CH));
		uart_dump((uint32_t)trans_intr.buffer, 0x20);
	}
}

usb_input_data *usb_input_get_data() {
	return (usb_input_data *)trans_intr.buffer;
}

