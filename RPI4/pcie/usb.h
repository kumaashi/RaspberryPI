#ifndef _USB_H_
#define _USB_H_

#include <stddef.h>

int usb_init();
void usb_check_intr();
int usb_open(int index);
size_t usb_read(int index, void *buffer, size_t size);
size_t usb_write(int index, void *buffer, size_t size);

#endif //_USB_H_
