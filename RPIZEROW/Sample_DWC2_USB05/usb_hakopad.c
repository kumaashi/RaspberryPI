#include "common.h"
#include "uart.h"
#include "hw.h"
#include "mailbox.h"
#include "usb.h"
#include "usb_hakopad.h"

static uint8_t usb_test_buffer[1024] __attribute__ ((aligned (256))) ;

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

	int addr = 0;
	int size = 0;
	int config = 1;

	//Should be obtain to hakopad descriptor interface and ep witu using lsusb -D
	//GET_DESCRIPTOR
	dwc2_clear_buffer_data(0x5A);
	dwc2_device_request(USB_DREQ_GET_DESCRIPTOR, addr, 0, config, size);
	SLEEP(WAIT_CNT);
	dwc2_print_reg();
	SLEEP(WAIT_CNT);
	dwc2_host_clear_int();

	//SET_ADDRESS
	addr = ADDR;
	dwc2_clear_buffer_data(0x5A);
	dwc2_device_request(USB_DREQ_SET_ADDRESS, 0, addr, 0, size);
	SLEEP(WAIT_CNT);
	dwc2_print_reg();
	SLEEP(WAIT_CNT);
	dwc2_host_clear_int();

	//SET_CONFIGURATION
	dwc2_clear_buffer_data(0x5A);
	dwc2_device_request(USB_DREQ_SET_CONFIGURATION, addr, config, 0, size);
	SLEEP(WAIT_CNT);
	dwc2_print_reg();
	SLEEP(WAIT_CNT);
	dwc2_host_clear_int();

	return 0;
}

void hakopad_update() {
	dwc2_usb_in_intr(CH, (void *)usb_test_buffer, 0x20, ADDR, 1, 1);
}

hakopad_data *hakopad_get_data() {
	//uart_debug_puts("*USB_HCINT(2)=", *USB_HCINT(CH));
	//*USB_HCINT(2) |= 0x7FF;
	return (hakopad_data *)usb_test_buffer;
}

