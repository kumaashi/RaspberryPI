#include "common.h"
#include "hw.h"
#include "uart.h"
#include "mailbox.h"
#include "led.h"


/*
void enable_intr_timer_irq(void) {
	PUT32(TIMER_LOAD, 0);
	PUT32(INTR_ENABLE_BASIC_IRQS, (1 << 0) );
	PUT32(TIMER_CONTROL,
	      (1 << 1) | //23-bit counter
	      (1 << 5) | //Timer interrupt enabled
	      (1 << 7) | //Timer enabled
	      (1 << 9) | //Free running counter enabled
	      0);
}

int notmain ( void )
{
	uint32_t ra;

	ENABLE_IRQ();

	enable_fn_led();
	enable_intr_timer_irq();

	while(1)
	{
		set_led(1);
		for(ra=0; ra<0x100000; ra++) SLEEP(ra);
		set_led(0);
		for(ra=0; ra<0x100000; ra++) SLEEP(ra);
	}

	return(0);
}
*/

//fake_vsync_isr=1

uint32_t g_count = 0;
uint32_t g_frame_index = 0;
uint32_t get_irq_flip_count() {
	return g_count;
}
void enable_irq_flip_isr(uint32_t frame_index, uint32_t count) {
	g_count = count;
	g_frame_index = frame_index;
	*IRQ_GPU_ENABLE2 |= IRQ_GPU_FAKE_ISR;
}

void fake_vsync(void) {
	while( (*IRQ_GPU_PENDING2 & IRQ_GPU_FAKE_ISR) == 0 );
	*SMI_CS = 0;
	*IRQ_GPU_ENABLE2 &= ~IRQ_GPU_FAKE_ISR;
}

void intr_handler() {
	uart_puts("call intr_handler\n");
	DISABLE_IRQ();
	if(*IRQ_GPU_PENDING2 & IRQ_GPU_FAKE_ISR) {
		*SMI_CS = 0;
		//*IRQ_GPU_ENABLE2 &= ~IRQ_GPU_FAKE_ISR;
		mailbox_fb_flip(g_frame_index);
		uart_puts("clear SMI INT\n");
	}
}

void handle_hang(const char *name) {
	led_init();
	uart_init();
	while(1) {
		uart_puts("HANG!!!!!!!!!\n");
		uart_puts(name);
		SLEEP(0x1000000);
	}
}

void handle_undef() {
	handle_hang("UNDEF ");
}

void handle_prefa() {
	handle_hang("PREFETCH ABORT ");
}

void handle_dataa() {
	handle_hang("DATA ABORT ");
}
