#pragma once

#define USB_DREQ_CLEAR_FEATURE     (0x01) //breq : 0000 0000b
#define USB_DREQ_GET_CONFIGURATION (0x08) //breq : 1000 0000b
#define USB_DREQ_GET_DESCRIPTOR    (0x06) //breq : 1000 0000b
#define USB_DREQ_GET_STATUS        (0x00) //breq : 1000 0000b
#define USB_DREQ_SET_ADDRESS       (0x05) //breq : 0000 0000b
#define USB_DREQ_SET_CONFIGURATION (0x09) //breq : 0000 0000b
#define USB_DREQ_SET_DESCRIPTOR    (0x07) //breq : 0000 0000b
#define USB_DREQ_SET_FEATURE       (0x03) //breq : 0000 0000b
#define USB_IREQ_CLEAR_FEATURE     (0x01) //breq : 0000 0001b //Feature Selector	Interface	Zero	None
#define USB_IREQ_GET_INTERFACE     (0x0A) //breq : 1000 0001b //Zero	Interface	One	Alternate Interface
#define USB_IREQ_GET_STATUS        (0x00) //breq : 1000 0001b //Zero	Interface	Two	Interface Status
#define USB_IREQ_SET_FEATURE       (0x03) //breq : 0000 0001b //Feature Selector	Interface	Zero	None
#define USB_IREQ_SET_INTERFACE     (0x0B) //breq : 0000 0001b //
#define USB_REQ_IN                 (0x80)
#define USB_REQ_OUT                (0x00)

void dwc2_clear_buffer_data(uint8_t filldata);
void dwc2_core_reset();
void dwc2_device_request(int req, int addr, uint16_t value, uint16_t index, int size);
void dwc2_host_clear_global_int();
void dwc2_host_clear_int();
void dwc2_hprt_poweron_reset();
void dwc2_interface_request(int req, int addr, uint16_t value, uint16_t index);
void dwc2_print_reg();
void dwc2_usb_in_intr(int ch, void *buffer, uint8_t len, int dev_addr, int epnum, int isin);
