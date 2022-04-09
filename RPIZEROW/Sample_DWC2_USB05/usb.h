//------------------------------------------------------------------------------
// rpiusb
//------------------------------------------------------------------------------
#ifndef _USB_H_
#define _USB_H_

//usb_20.pdf
#define USB_DREQ_CLEAR_FEATURE     (0x01) //breq : 0000 0000b
#define USB_DREQ_GET_CONFIGURATION (0x08) //breq : 1000 0000b
#define USB_DREQ_GET_DESCRIPTOR    (0x06) //breq : 1000 0000b
#define USB_DREQ_GET_STATUS        (0x00) //breq : 1000 0000b
#define USB_DREQ_SET_ADDRESS       (0x05) //breq : 0000 0000b
#define USB_DREQ_SET_CONFIGURATION (0x09) //breq : 0000 0000b
#define USB_DREQ_SET_DESCRIPTOR    (0x07) //breq : 0000 0000b
#define USB_DREQ_SET_FEATURE       (0x03) //breq : 0000 0000b
#define USB_IREQ_CLEAR_FEATURE     (0x01) //breq : 0000 0001b
#define USB_IREQ_GET_INTERFACE     (0x0A) //breq : 1000 0001b
#define USB_IREQ_GET_STATUS        (0x00) //breq : 1000 0001b
#define USB_IREQ_SET_FEATURE       (0x03) //breq : 0000 0001b
#define USB_IREQ_SET_INTERFACE     (0x0B) //breq : 0000 0001b

#define USB_REQ_IN                 (0x80)
#define USB_REQ_OUT                (0x00)

#define USB_HCINT_DATATGLERR       (1 << 10)
#define USB_HCINT_FRMOVRUN         (1 << 9)
#define USB_HCINT_BBLERR           (1 << 8)
#define USB_HCINT_XACTERR          (1 << 7)
#define USB_HCINT_ACK              (1 << 5)
#define USB_HCINT_NAK              (1 << 4)
#define USB_HCINT_STALL            (1 << 3)
#define USB_HCINT_AHBERR           (1 << 2)
#define USB_HCINT_CHHLTD           (1 << 1)
#define USB_HCINT_XFERCOMPL        (1 << 0)
#define USB_BUF_SIZE               (512)

typedef struct rpiusb_request_t {
	uint8_t bmRequestType; 
	uint8_t bRequest; 
	uint16_t wValue; 
	uint16_t wIndex; 
	uint16_t wLength; 
	uint8_t *out_buffer;
	uint8_t *in_buffer;
} __attribute__((__packed__)) rpiusb_request;

typedef struct rpiusb_trans_data_t {
	int ch;
	void *buffer;
	uint8_t len;
	int dev_addr;
	int epnum;
	int isin;
	int isdata1; //toggle

	uint8_t *out_buffer;
	uint8_t *in_buffer;
} __attribute__((__packed__)) rpiusb_trans_data;

void rpiusb_core_reset();
void rpiusb_hprt_poweron_reset();

void rpiusb_device_request(int req, int addr, uint16_t value, uint16_t index, int size);
void rpiusb_device_request_raw(rpiusb_request *req);
void rpiusb_interface_request(int req, int addr, uint16_t value, uint16_t index);
void rpiusb_interface_request_raw(rpiusb_request *req);

void rpiusb_hc_clear_global_int();
void rpiusb_hc_clear_int(int ch);

void rpiusb_trans_intr(int ch, void *buffer, uint8_t len, int dev_addr, int epnum, int isin, int isdata1);
void rpiusb_trans_intr_raw(rpiusb_trans_data *pdata);
void rpiusb_dump_ctrl_buffer();
void rpiusb_print_reg();

uint32_t rpiusb_hc_get_stall(int ch);
uint32_t rpiusb_hc_get_nak(int ch);
uint32_t rpiusb_hc_get_ack(int ch);
uint32_t rpiusb_hc_get_ch_halt(int ch);
uint32_t rpiusb_hc_get_xfer_compl(int ch);

#endif // _USB_H_


