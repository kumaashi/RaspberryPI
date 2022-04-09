#include "common.h"
#include "uart.h"
#include "hw.h"
#include "mailbox.h"
#include "usb.h"
#include "usb_usb_input.h"
#include "heap.h"

static uint8_t *usb_input_buffer;

#define CH   2
#define ADDR 1

int usb_input_init(void) {
	//https://github.com/raspberrypi/linux/blob/aeaa2460db088fb2c97ae56dec6d7d0058c68294/drivers/soc/bcm/raspberrypi-power.c#L20
	const int POWER_DOMAIN_USB = 3;
	mailbox_set_power_state(POWER_DOMAIN_USB, 1);

	//reset core
	rpiusb_core_reset();

	//reset port
	rpiusb_hprt_poweron_reset();

	usb_input_buffer = (uint8_t *)heap_get();

	int addr = 0;
	uint16_t value;
	uint16_t index;
	int size = 0;
	int config = 0;

	//----------------------------------------------------
	//D - SET_ADDRESS

	index = 0;
	size = 0x0;

	rpiusb_clear_buffer_data(0xCC);
	rpiusb_device_request(USB_DREQ_SET_ADDRESS, 0, ADDR, index, size);
	SLEEP(WAIT_CNT);
	rpiusb_print_reg();
	SLEEP(WAIT_CNT);
	rpiusb_dump_ctrl_buffer();
	rpiusb_host_clear_int();
	addr = ADDR;

	//----------------------------------------------------
	//Should be preparing to obtain to usb_input descriptor interface and ep witu using lsusb -D
	//D - GET_DESCRIPTOR
	value = 0;
	index = 1;
	size = 0;

	rpiusb_clear_buffer_data(0xCC);
	rpiusb_device_request(USB_DREQ_GET_DESCRIPTOR, addr, value, index, size);
	SLEEP(WAIT_CNT);
	rpiusb_print_reg();
	rpiusb_dump_ctrl_buffer();
	SLEEP(WAIT_CNT);
	rpiusb_host_clear_int();

	//----------------------------------------------------
	//D - GET_CONFIGURATION
	value = 0;
	index = 0;
	size = 0;

	rpiusb_clear_buffer_data(0xCC);
	rpiusb_device_request(USB_DREQ_GET_CONFIGURATION, addr, value, index, size);
	SLEEP(WAIT_CNT);
	rpiusb_print_reg();
	SLEEP(WAIT_CNT);
	rpiusb_dump_ctrl_buffer();
	rpiusb_host_clear_int();

	//----------------------------------------------------
	//D - SET_CONFIGURATION
	config = 1;
	value = config;
	index = 0;
	size = 0x0;

	rpiusb_clear_buffer_data(0xCC);
	rpiusb_device_request(USB_DREQ_SET_CONFIGURATION, addr, value, index, size);
	SLEEP(WAIT_CNT);
	rpiusb_print_reg();
	SLEEP(WAIT_CNT);
	rpiusb_dump_ctrl_buffer();
	rpiusb_host_clear_int();

	//----------------------------------------------------
	//D - GET_CONFIGURATION
	value = 0;
	index = 0;
	size = 0;

	rpiusb_clear_buffer_data(0xCC);
	rpiusb_device_request(USB_DREQ_GET_CONFIGURATION, addr, value, index, size);
	SLEEP(WAIT_CNT);
	rpiusb_print_reg();
	SLEEP(WAIT_CNT);
	rpiusb_dump_ctrl_buffer();
	rpiusb_host_clear_int();


	//----------------------------------------------------
	//I - SET_INTERFACE
	rpiusb_clear_buffer_data(0xCC);
	rpiusb_interface_request(USB_IREQ_SET_INTERFACE, addr, 0, 0);
	SLEEP(WAIT_CNT);
	rpiusb_print_reg();
	SLEEP(WAIT_CNT);
	rpiusb_dump_ctrl_buffer();
	rpiusb_host_clear_int();

	//----------------------------------------------------
	//I - GET_INTERFACE
	rpiusb_clear_buffer_data(0xCC);
	rpiusb_interface_request(USB_IREQ_GET_INTERFACE, addr, 0, 1);
	SLEEP(WAIT_CNT);
	rpiusb_print_reg();
	SLEEP(WAIT_CNT);
	rpiusb_dump_ctrl_buffer();
	rpiusb_host_clear_int();

	//ping
	rpiusb_usb_intr(CH, (void *)usb_input_buffer, 0x20, ADDR, 1, 1, 0);
	return 0;
}

static int epindex = 1;
static int toggle = 0;
void usb_input_update() {
	if(rpiusb_usb_get_ch_halt(CH) == 0) {
		return;
	}
	//initialize
	//static int is_init = 0;


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

	*USB_HCINT(CH) |= 0x7FF;
	rpiusb_usb_intr(CH, (void *)usb_input_buffer, 0x20, ADDR, epindex, 1, toggle & 1);

	//tentative wait for halt
	while(rpiusb_usb_get_ch_halt(CH) == 0) {
		SLEEP(2);
	}

	if(!rpiusb_usb_get_nak(CH) || !rpiusb_usb_get_stall())
		toggle++;

	//debug
	uint32_t status = *USB_HCINT(CH);
	if(status & USB_HCINT_DATATGLERR)
		uart_puts("USB_HCINT_DATATGLERR\n");
	if(status & USB_HCINT_BBLERR)
		uart_puts("USB_HCINT_BBLERR\n");
	if(status & USB_HCINT_XACTERR)
		uart_puts("USB_HCINT_XACTERR\n");
	static int count = 0;
	count++;
	if((count % 60) == 0)
	{
		uart_debug_puts("hcint2=", *USB_HCINT(CH));
		uart_dump((uint32_t)usb_input_buffer, 0x20);
	}
}

usb_input_data *usb_input_get_data() {
	//debug
	return (usb_input_data *)usb_input_buffer;
}

