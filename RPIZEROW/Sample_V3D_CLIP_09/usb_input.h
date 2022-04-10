#ifndef _USB_INPUT_H_
#define _USB_INPUT_H_

//http://free60.org/wiki/GamePad
//http://euc.jp/periphs/xbox-controller.ja.html

typedef struct usb_input_data_t {
	unsigned Message_type          : 8;
	unsigned Packet_size           : 8;
	unsigned D_Pad_up              : 1;
	unsigned D_Pad_down            : 1;
	unsigned D_Pad_left            : 1;
	unsigned D_Pad_right           : 1;
	unsigned Start_button          : 1;
	unsigned Back_button           : 1;
	unsigned Left_stick_press      : 1;
	unsigned Right_stick_press     : 1;
	unsigned Button_LB             : 1;
	unsigned Button_RB             : 1;
	unsigned Xbox_logo_button      : 1;
	unsigned Unused                : 1;
	unsigned Button_A              : 1;
	unsigned Button_B              : 1;
	unsigned Button_X              : 1;
	unsigned Button_Y              : 1;
	unsigned Left_trigger          : 8;
	unsigned Right_trigger         : 8;
	int16_t Left_stick_X_axis     ;
	int16_t Left_stick_Y_axis     ;
	int16_t Right_stick_X_axis    ;
	int16_t Right_stick_Y_axis    ;
} usb_input_data;

int usb_input_init(void);
void usb_input_update();
usb_input_data *usb_input_get_data();


#endif //_USB_INPUT_H_
