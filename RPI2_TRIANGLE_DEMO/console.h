#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "uart.h"

void console_reset();
void console_update(void (*func)(const uint8_t *cmd));

struct command_data {
	char *name;
	void (*func)(uint8_t *pcmd);
};

#endif

