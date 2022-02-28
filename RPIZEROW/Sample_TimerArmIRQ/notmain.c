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

#define USB_EX_BASE                (0x20980000)
#define USB_EX_MDIO_CNTL           (USB_EX_BASE + 0x080)
#define USB_EX_MDIO_DATA           (USB_EX_BASE + 0x084)
#define USB_EX_MDIO_VBUS           (USB_EX_BASE + 0x088)

#define HSOTG_REG(x)			(USB_EX_BASE + x)

/* global */
#define USB_GOTGCTL			HSOTG_REG(0x000)
#define USB_GOTGINT			HSOTG_REG(0x004)
#define USB_GAHBCFG			HSOTG_REG(0x008)
#define USB_GUSBCFG			HSOTG_REG(0x00C)
#define USB_GRSTCTL			HSOTG_REG(0x010)
#define USB_GINTSTS			HSOTG_REG(0x014)
#define USB_GINTMSK			HSOTG_REG(0x018)
#define USB_GRXSTSR			HSOTG_REG(0x01C)
#define USB_GRXSTSP			HSOTG_REG(0x020)
#define USB_GRXFSIZ			HSOTG_REG(0x024)
#define USB_GNPTXFSIZ		HSOTG_REG(0x028)
#define USB_GNPTXSTS		HSOTG_REG(0x02C)
#define USB_GI2CCTL			HSOTG_REG(0x0030)
#define USB_GPVNDCTL		HSOTG_REG(0x0034)
#define USB_GGPIO				HSOTG_REG(0x0038)
#define USB_GUID				HSOTG_REG(0x003c)
#define USB_GSNPSID			HSOTG_REG(0x0040)
#define USB_GHWCFG1			HSOTG_REG(0x0044)
#define USB_GHWCFG2			HSOTG_REG(0x0048)
#define USB_GHWCFG3			HSOTG_REG(0x004c)
#define USB_GHWCFG4			HSOTG_REG(0x0050)
#define USB_GLPMCFG			HSOTG_REG(0x0054)
#define USB_GPWRDN			HSOTG_REG(0x0058)
#define USB_GDFIFOCFG		HSOTG_REG(0x005c)
#define USB_GPTXFSIZ		HSOTG_REG(0x0100)

#define USB_HCFG				HSOTG_REG(0x0400)
#define USB_HFIR				HSOTG_REG(0x0404)
#define USB_HFNUM				HSOTG_REG(0x0408)
#define USB_HPTXSTS			HSOTG_REG(0x0410)
#define USB_HAINT				HSOTG_REG(0x0414)
#define USB_HAINTMSK		HSOTG_REG(0x0418)
#define USB_HFLBADDR		HSOTG_REG(0x041c)
#define USB_HPRT0				HSOTG_REG(0x0440)
#define USB_HCCHAR(_ch)			HSOTG_REG(0x0500 + 0x20 * (_ch))
#define USB_HCSPLT(_ch)			HSOTG_REG(0x0504 + 0x20 * (_ch))
#define USB_HCINT(_ch)			HSOTG_REG(0x0508 + 0x20 * (_ch))
#define USB_HCINTMSK(_ch)		HSOTG_REG(0x050c + 0x20 * (_ch))
#define USB_HCTSIZ(_ch)			HSOTG_REG(0x0510 + 0x20 * (_ch))
#define USB_HCDMA(_ch)			HSOTG_REG(0x0514 + 0x20 * (_ch))
#define USB_HCDMAB(_ch)			HSOTG_REG(0x051c + 0x20 * (_ch))

/* misc control */
#define USB_PCGCTL			HSOTG_REG(0x0e00)
#define USB_PCGCCTL1		HSOTG_REG(0xe04)
#define USB_HCFIFO(_ch)	HSOTG_REG(0x1000 + 0x1000 * (_ch))

/* misc */
#define USB_EPFIFO(_a)	HSOTG_REG(0x1000 + ((_a) * 0x1000))

/*
void init_usb_test(void) {
	PUT32(USB_HPRT0, 0);

	PUT32(USB_EX_MDIO_VBUS, 0x0000000e);
	PUT32(USB_EX_MDIO_CNTL, (1 << 18));

	//SET POWER
	PUT32(USB_HPRT0, GET32(USB_HPRT0) | (1 << 12));
	for(ra=0; ra<0x100000; ra++) SLEEP(ra);

	//CONN_DET
	PUT32(USB_HPRT0, GET32(USB_HPRT0) | (1 << 1));
	for(ra=0; ra<0x100000; ra++) SLEEP(ra);

	//RESET
	PUT32(USB_HPRT0, GET32(USB_HPRT0) | (1 << 8));
	for(ra=0; ra<0x100000; ra++) SLEEP(ra);

	//RESET deassert
	PUT32(USB_HPRT0, GET32(USB_HPRT0) & ~(1 << 8));
	for(ra=0; ra<0x100000; ra++) SLEEP(ra);

	//SET EN CHANGE
	PUT32(USB_HPRT0, GET32(USB_HPRT0) | (1 << 3));
	for(ra=0; ra<0x100000; ra++) SLEEP(ra);

	//SET FULL SPEED
	//PUT32(USB_HPRT0, GET32(USB_HPRT0) | (1 << 17));
	PUT32(USB_HPRT0, GET32(USB_HPRT0) | (0 << 17));
	for(ra=0; ra<0x100000; ra++) SLEEP(ra);

  //PUT32(USB_GAHBCFG, 0x0000000e);
	//for(ra=0; ra<0x100000; ra++) SLEEP(ra);
  //PUT32(USB_GUSBCFG, 0x20402700);
	//for(ra=0; ra<0x100000; ra++) SLEEP(ra);

	//SET SIZE
  PUT32(USB_GRXFSIZ, 0x00001000);
	for(ra=0; ra<0x100000; ra++) SLEEP(ra);

}
*/

int wait_bit(uint32_t addr, uint32_t mask) {
	int count = 0;
	
	for( count = 0 ; count < 0x100000; count++) {
		if((GET32(addr) & mask) == mask)
			return 0;
	}
	return -1;
}

void sleep_clk(int a) {
	while(a-- > 0)
		SLEEP(ra);
}


void init_usb_0(void) {

	uint32_t ra;
	
	//setup clock init
	//PUT32(USB_EX_MDIO_VBUS, 0x00000002);
	PUT32(USB_PCGCTL, 0);
	sleep_clk(0x100000);

	PUT32(USB_GRSTCTL, GET32(USB_GRSTCTL) | (1 << 0));
	sleep_clk(0x100000);

	PUT32(USB_GRSTCTL, GET32(USB_GRSTCTL) | (1 << 29));
	sleep_clk(0x100000);

	//PUT32(USB_GUSBCFG, GET(USB_GUSBCFG) & ~( (1 << 17) | (1 << 19)));
	PUT32(USB_GRXFSIZ, 1024);
	PUT32(USB_GNPTXFSIZ, 0x01001000);
	PUT32(USB_GPTXFSIZ,  0x10000100);
	PUT32(USB_GOTGINT,  GET32(USB_GOTGCTL) & ~( 1<< 16));

	PUT32(USB_PCGCTL, 0);
	PUT32(USB_HPRT0, GET32(USB_HPRT0) & ~( (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5)));

	//SET POWER
	PUT32(USB_HPRT0, GET32(USB_HPRT0) | (1 << 12));
	sleep_clk(0x100000);

	////CONN_DET
	PUT32(USB_HPRT0, GET32(USB_HPRT0) | (1 << 1));
	sleep_clk(0x100000);

	//RESET
	//PUT32(USB_HPRT0, GET32(USB_HPRT0) | (1 << 8));
	PUT32(USB_HPRT0, GET32(USB_HPRT0) |  ( (1 << 8) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5)));
	sleep_clk(0x100000);

	//RESET deassert
	//PUT32(USB_HPRT0, GET32(USB_HPRT0) & ~(1 << 8));
	PUT32(USB_HPRT0, GET32(USB_HPRT0) & ~( (1 << 8) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5)));
	sleep_clk(0x100000);

	//test HC
	uint32_t hcchar = 0;
	hcchar |= (1 << 15); //IN
	hcchar |= (3 << 18); //Type Interrupt(3)
	hcchar |= (1 << 11); //EP 1
	hcchar |= 512;

	PUT32(USB_HCINT(0), 0x3fff); 
	PUT32(USB_HCCHAR(0), hcchar); 
	PUT32(USB_HCSPLT(0), 0); 
}

int notmain ( void )
{
	uint32_t ra;

	ENABLE_IRQ();

	init_usb_0();

	//enable_fn_led();
	//enable_intr_timer_irq();

	int count;
	while(1)
	{
		if(GET32(USB_GINTSTS) & (1 << 18)) {
			count = 0x10000;
		} else {
			count = 0x100000;
		}

		set_led(1);
		for(ra=0; ra<count; ra++) SLEEP(ra);
		set_led(0);
		for(ra=0; ra<count; ra++) SLEEP(ra);
	}

	return(0);
}

