#include "common.h"
#include "uart.h"
#include "hw.h"
#include "mailbox.h"
#include "usb.h"
#include "usb_hakopad.h"
#include "heap.h"

static uint8_t *usb_input_buffer;

#define CH 2
#define ADDR 1

int hakopad_init(void) {

	//https://github.com/raspberrypi/linux/blob/aeaa2460db088fb2c97ae56dec6d7d0058c68294/drivers/soc/bcm/raspberrypi-power.c#L20
	const int POWER_DOMAIN_USB = 3;
	mailbox_set_power_state(POWER_DOMAIN_USB, 1);

	//reset core
	dwc2_core_reset();

	//reset port
	dwc2_hprt_poweron_reset();

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

	dwc2_clear_buffer_data(0xCC);
	dwc2_device_request(USB_DREQ_SET_ADDRESS, 0, ADDR, index, size);
	SLEEP(WAIT_CNT);
	dwc2_print_reg();
	SLEEP(WAIT_CNT);
	dwc2_dump_ctrl_buffer();
	dwc2_host_clear_int();
	addr = ADDR;

	//----------------------------------------------------
	//Should be preparing to obtain to hakopad descriptor interface and ep witu using lsusb -D
	//D - GET_DESCRIPTOR
	value = 0;
	index = 1;
	size = 0;

	dwc2_clear_buffer_data(0xCC);
	dwc2_device_request(USB_DREQ_GET_DESCRIPTOR, addr, value, index, size);
	SLEEP(WAIT_CNT);
	dwc2_print_reg();
	dwc2_dump_ctrl_buffer();
	SLEEP(WAIT_CNT);
	dwc2_host_clear_int();


#if 0
	//----------------------------------------------------
	//D - GET_CONFIGURATION
	value = 0;
	index = 0;
	size = 0;

	dwc2_clear_buffer_data(0xCC);
	dwc2_device_request(USB_DREQ_GET_CONFIGURATION, addr, value, index, size);
	SLEEP(WAIT_CNT);
	dwc2_print_reg();
	SLEEP(WAIT_CNT);
	dwc2_dump_ctrl_buffer();
	dwc2_host_clear_int();
#endif
	//----------------------------------------------------
	//D - SET_CONFIGURATION
	config = 1;
	value = config;
	index = 0;
	size = 0x0;

	dwc2_clear_buffer_data(0xCC);
	dwc2_device_request(USB_DREQ_SET_CONFIGURATION, addr, value, index, size);
	SLEEP(WAIT_CNT);
	dwc2_print_reg();
	SLEEP(WAIT_CNT);
	dwc2_dump_ctrl_buffer();
	dwc2_host_clear_int();

	//----------------------------------------------------
	//D - GET_CONFIGURATION
	value = 0;
	index = 0;
	size = 0;

	dwc2_clear_buffer_data(0xCC);
	dwc2_device_request(USB_DREQ_GET_CONFIGURATION, addr, value, index, size);
	SLEEP(WAIT_CNT);
	dwc2_print_reg();
	SLEEP(WAIT_CNT);
	dwc2_dump_ctrl_buffer();
	dwc2_host_clear_int();


	//----------------------------------------------------
	//I - SET_INTERFACE
	dwc2_clear_buffer_data(0xCC);
	dwc2_interface_request(USB_IREQ_SET_INTERFACE, addr, 0, 0);
	SLEEP(WAIT_CNT);
	dwc2_print_reg();
	SLEEP(WAIT_CNT);
	dwc2_dump_ctrl_buffer();
	dwc2_host_clear_int();

	//----------------------------------------------------
	//I - GET_INTERFACE
	dwc2_clear_buffer_data(0xCC);
	dwc2_interface_request(USB_IREQ_GET_INTERFACE, addr, 0, 1);
	SLEEP(WAIT_CNT);
	dwc2_print_reg();
	SLEEP(WAIT_CNT);
	dwc2_dump_ctrl_buffer();
	dwc2_host_clear_int();
#if 0
#endif

	return 0;
}

void hakopad_update() {
	static int epindex = 1;

	//initialize
	static int is_init = 0;

	*USB_HCINT(CH) |= 0x7FF;
	if(is_init == 0) {
		usb_input_buffer[0] = 0x05;
		usb_input_buffer[1] = 0x20;
		usb_input_buffer[2] = 0x00;
		usb_input_buffer[3] = 0x01;
		usb_input_buffer[4] = 0x00;
		dwc2_usb_intr(CH, (void *)usb_input_buffer, 0x5, ADDR, epindex, 0);
		is_init = 1;
	} else {
		dwc2_usb_intr(CH, (void *)usb_input_buffer, 0x20, ADDR, epindex, 1);
	}

	//debug
	uart_debug_puts("hcint2=", *USB_HCINT(CH));

	uart_dump((uint32_t)usb_input_buffer, 0x20);
}

hakopad_data *hakopad_get_data() {
	return (hakopad_data *)usb_input_buffer;
}

