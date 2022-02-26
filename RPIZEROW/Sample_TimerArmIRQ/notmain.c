// Raspberry Pi ARM Timer IRQ sample
// 2022 yasai kumaashi (gyaboyan@gmail.com)
// ref:https://github.com/dwelch67/raspberrypi-zero/
#include <stdint.h>

extern void ENABLE_IRQ( void );
extern void PUT32 ( uint32_t, uint32_t );
extern uint32_t GET32 ( uint32_t );
extern void SLEEP ( uint32_t );

#define GPFSEL3                    0x2020000C
#define GPFSEL4                    0x20200010
#define GPSET1                     0x20200020
#define GPCLR1                     0x2020002C

#define INTR_BASE                  0x2000B000
#define INTR_IRQ_BASIC_PENDING     (INTR_BASE + 0x200)
#define INTR_IRQ_PENDING_1         (INTR_BASE + 0x204)
#define INTR_IRQ_PENDING_2         (INTR_BASE + 0x208)
#define INTR_FIQ_CONTROL           (INTR_BASE + 0x20C)
#define INTR_ENABLE_IRQS_1         (INTR_BASE + 0x210)
#define INTR_ENABLE_IRQS_2         (INTR_BASE + 0x214)
#define INTR_ENABLE_BASIC_IRQS     (INTR_BASE + 0x218)
#define INTR_DISABLE_IRQS_1        (INTR_BASE + 0x21C)
#define INTR_DISABLE_IRQS_2        (INTR_BASE + 0x220)
#define INTR_DISABLE_BASIC_IRQS    (INTR_BASE + 0x224)

#define TIMER_BASE                 (0x2000B000)
#define TIMER_LOAD                 (TIMER_BASE + 0x400) // Load
#define TIMER_VALUE                (TIMER_BASE + 0x404) // Value (Read Only)
#define TIMER_CONTROL              (TIMER_BASE + 0x408) // Control
#define TIMER_IRQ_CLEAR_ACK        (TIMER_BASE + 0x40C) // IRQ Clear/Ack (Write only)
#define TIMER_RAW_IRQ              (TIMER_BASE + 0x410) // RAW IRQ (Read Only)
#define TIMER_MASKED_IRQ           (TIMER_BASE + 0x414) // Masked IRQ (Read Only)
#define TIMER_RELOAD               (TIMER_BASE + 0x418) // Reload
#define TIMER_PRE_DIVIDER          (TIMER_BASE + 0x41C) // Pre-divider (Not in real 804!)
#define TIMER_FREE_RUNNING_COUNTER (TIMER_BASE + 0x420) // Free running counter (Not in real 804!)

void fn_intr_irq(void) {
	uint32_t count;
	uint32_t ra;

	PUT32(TIMER_IRQ_CLEAR_ACK, (1 << 0));

	for(count = 0 ; count < 10; count++)
	{
		PUT32(GPSET1,1<<(47-32));
		for(ra=0; ra<0x20000; ra++) SLEEP(ra);
		PUT32(GPCLR1,1<<(47-32));
		for(ra=0; ra<0x20000; ra++) SLEEP(ra);
	}

	PUT32(TIMER_LOAD, (1 << 20) - 1);
	return;
}

void enable_fn_led() {
	uint32_t val;

	val = GET32(GPFSEL4);
	val &= ~(7 << 21);
	val |= (1 << 21);
	PUT32(GPFSEL4,val);
}

void set_led(int a) {
	if(a) {
		PUT32(GPSET1,1<<(47-32));
	} else {
		PUT32(GPCLR1,1<<(47-32));
	}
}

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

