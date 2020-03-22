#include "std.h"
#include "uart.h"
#include "usb.h"
#include "hwio.h"


#define HSOTG_REG(x)			(SUBSYSTEM_BASE + 0x980000 + x)

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
#define USB_GNPTXFSIZ			HSOTG_REG(0x028)
#define USB_GNPTXSTS			HSOTG_REG(0x02C)
#define USB_GI2CCTL			HSOTG_REG(0x0030)
#define USB_GPVNDCTL			HSOTG_REG(0x0034)
#define USB_GGPIO			HSOTG_REG(0x0038)
#define USB_GUID			HSOTG_REG(0x003c)
#define USB_GSNPSID			HSOTG_REG(0x0040)
#define USB_GHWCFG1			HSOTG_REG(0x0044)
#define USB_GHWCFG2			HSOTG_REG(0x0048)
#define USB_GHWCFG3			HSOTG_REG(0x004c)
#define USB_GHWCFG4			HSOTG_REG(0x0050)
#define USB_GLPMCFG			HSOTG_REG(0x0054)
#define USB_GPWRDN			HSOTG_REG(0x0058)
#define USB_GDFIFOCFG			HSOTG_REG(0x005c)

/* misc control */
#define USB_ADPCTL			HSOTG_REG(0x0060)
#define USB_HPTXFSIZ			HSOTG_REG(0x100)

/* device */
#define USB_DPTXFSIZN(_a)		HSOTG_REG(0x104 + (((_a) - 1) * 4))
#define USB_DCFG			HSOTG_REG(0x800)
#define USB_DCTL			HSOTG_REG(0x804)
#define USB_DSTS			HSOTG_REG(0x808)
#define USB_DIEPMSK			HSOTG_REG(0x810)
#define USB_DOEPMSK			HSOTG_REG(0x814)
#define USB_DAINT			HSOTG_REG(0x818)
#define USB_DAINTMSK			HSOTG_REG(0x81C)
#define USB_DTKNQR1			HSOTG_REG(0x820)
#define USB_DTKNQR2			HSOTG_REG(0x824)
#define USB_DTKNQR3			HSOTG_REG(0x830)
#define USB_DTKNQR4			HSOTG_REG(0x834)
#define USB_DIEPEMPMSK			HSOTG_REG(0x834)
#define USB_DVBUSDIS			HSOTG_REG(0x828)
#define USB_DVBUSPULSE			HSOTG_REG(0x82C)
#define USB_DIEPCTL0			HSOTG_REG(0x900)
#define USB_DIEPCTL(_a)			HSOTG_REG(0x900 + ((_a) * 0x20))
#define USB_DOEPCTL0			HSOTG_REG(0xB00)
#define USB_DOEPCTL(_a)			HSOTG_REG(0xB00 + ((_a) * 0x20))
#define USB_DIEPINT(_a)			HSOTG_REG(0x908 + ((_a) * 0x20))
#define USB_DOEPINT(_a)			HSOTG_REG(0xB08 + ((_a) * 0x20))
#define USB_DIEPTSIZ0			HSOTG_REG(0x910)
#define USB_DOEPTSIZ0			HSOTG_REG(0xB10)
#define USB_DIEPTSIZ(_a)		HSOTG_REG(0x910 + ((_a) * 0x20))
#define USB_DOEPTSIZ(_a)		HSOTG_REG(0xB10 + ((_a) * 0x20))
#define USB_DIEPDMA(_a)			HSOTG_REG(0x914 + ((_a) * 0x20))
#define USB_DOEPDMA(_a)			HSOTG_REG(0xB14 + ((_a) * 0x20))
#define USB_DTXFSTS(_a)			HSOTG_REG(0x918 + ((_a) * 0x20))



/* host control */
#define USB_HCFG			HSOTG_REG(0x0400)
#define USB_HFIR			HSOTG_REG(0x0404)
#define USB_HFNUM			HSOTG_REG(0x0408)
#define USB_HPTXSTS			HSOTG_REG(0x0410)
#define USB_HAINT			HSOTG_REG(0x0414)
#define USB_HAINTMSK			HSOTG_REG(0x0418)
#define USB_HFLBADDR			HSOTG_REG(0x041c)
#define USB_HPRT0			HSOTG_REG(0x0440)
#define USB_HCCHAR(_ch)			HSOTG_REG(0x0500 + 0x20 * (_ch))
#define USB_HCSPLT(_ch)			HSOTG_REG(0x0504 + 0x20 * (_ch))
#define USB_HCINT(_ch)			HSOTG_REG(0x0508 + 0x20 * (_ch))
#define USB_HCINTMSK(_ch)		HSOTG_REG(0x050c + 0x20 * (_ch))
#define USB_HCTSIZ(_ch)			HSOTG_REG(0x0510 + 0x20 * (_ch))
#define USB_HCDMA(_ch)			HSOTG_REG(0x0514 + 0x20 * (_ch))
#define USB_HCDMAB(_ch)			HSOTG_REG(0x051c + 0x20 * (_ch))
/* misc control */
#define USB_PCGCTL			HSOTG_REG(0x0e00)
#define USB_PCGCCTL1			HSOTG_REG(0xe04)
#define USB_HCFIFO(_ch)			HSOTG_REG(0x1000 + 0x1000 * (_ch))

/* misc */
#define USB_EPFIFO(_a)			HSOTG_REG(0x1000 + ((_a) * 0x1000))

#define USB_GAHBCFG_EN_INTR		(1 << 0)
#define USB_GAHBCFG_WAIT_AXI_WRITES	(1 << 4)
#define USB_GAHBCFG_EN_DMA		(1 << 5)
#define USB_GAHBCFG_MAX_AXI_BURST__MASK	(3 << 1)
#define USB_GRSTCTL_IDLE		(1 << 31)
#define USB_GRSTCTL_SOFT_RESET		(1 << 0)
#define USB_GRSTCTL_RXFLUSH		(1 << 5)
#define USB_GRSTCTL_TXFLUSH		(1 << 4)

#define USB_GUSBCFG_PHYIF		(1 << 3)
#define USB_GUSBCFG_ULPI_UTMI_SEL	(1 << 4)
#define USB_GUSBCFG_SRP_CAPABLE 		(1 << 8)
#define USB_GUSBCFG_HNP_CAPABLE 	(1 << 9)
#define USB_GUSBCFG_ULPI_FSLS		(1 << 17)
#define USB_GUSBCFG_ULPI_CLK_SUS_M	(1 << 19)
#define USB_GUSBCFG_ULPI_EXT_VBUS_DRV	(1 << 20)
#define USB_GUSBCFG_TERM_SEL_DL_PULSE	(1 << 22)
	
#define USB_HCFG_FSLS_PCLK_SEL__MASK	(3 << 0)
#define USB_HCFG_FSLS_PCLK_SEL_30_60_MHZ	0
#define USB_HCFG_FSLS_PCLK_SEL_48_MHZ		1
#define USB_HCFG_FSLS_PCLK_SEL_6_MHZ		2
	
#define USB_GINTSTS_WKUPINT		(1 << 31)
#define USB_GINTSTS_SESSREQINT		(1 << 30)
#define USB_GINTSTS_DISCONNINT		(1 << 29)
#define USB_GINTSTS_CONIDSTSCHNG	(1 << 28)
#define USB_GINTSTS_LPMTRANRCVD		(1 << 27)
#define USB_GINTSTS_PTXFEMP		(1 << 26)
#define USB_GINTSTS_HCHINT		(1 << 25)
#define USB_GINTSTS_PRTINT		(1 << 24)
#define USB_GINTSTS_RESETDET		(1 << 23)
#define USB_GINTSTS_FET_SUSP		(1 << 22)
#define USB_GINTSTS_INCOMPL_IP		(1 << 21)
#define USB_GINTSTS_INCOMPL_SOOUT	(1 << 21)
#define USB_GINTSTS_INCOMPL_SOIN	(1 << 20)
#define USB_GINTSTS_OEPINT		(1 << 19)
#define USB_GINTSTS_IEPINT		(1 << 18)
#define USB_GINTSTS_EPMIS		(1 << 17)
#define USB_GINTSTS_RESTOREDONE		(1 << 16)
#define USB_GINTSTS_EOPF		(1 << 15)
#define USB_GINTSTS_ISOUTDROP		(1 << 14)
#define USB_GINTSTS_ENUMDONE		(1 << 13)
#define USB_GINTSTS_USBRST		(1 << 12)
#define USB_GINTSTS_USBSUSP		(1 << 11)
#define USB_GINTSTS_ERLYSUSP		(1 << 10)
#define USB_GINTSTS_I2CINT		(1 << 9)
#define USB_GINTSTS_ULPI_CK_INT		(1 << 8)
#define USB_GINTSTS_GOUTNAKEFF		(1 << 7)
#define USB_GINTSTS_GINNAKEFF		(1 << 6)
#define USB_GINTSTS_NPTXFEMP		(1 << 5)
#define USB_GINTSTS_RXFLVL		(1 << 4)
#define USB_GINTSTS_SOF			(1 << 3)
#define USB_GINTSTS_OTGINT		(1 << 2)
#define USB_GINTSTS_MODEMIS		(1 << 1)
#define USB_GINTSTS_CURMODE_HOST	(1 << 0)

#define USB_HPRT0_CONNECT		(1 << 0)
#define USB_HPRT0_CONNECT_CHANGED	(1 << 1)
#define USB_HPRT0_ENABLE		(1 << 2)
#define USB_HPRT0_ENABLE_CHANGED	(1 << 3)
#define USB_HPRT0_OVERCURRENT		(1 << 4)
#define USB_HPRT0_OVERCURRENT_CHANGED	(1 << 5)
#define USB_HPRT0_RESET			(1 << 8)
#define USB_HPRT0_POWER			(1 << 12)
#define USB_HPRT0_SPEED(reg)		(((reg) >> 17) & 3)
#define USB_HPRT0_SPEED_HIGH		(0)
#define USB_HPRT0_SPEED_FULL		(1)
#define USB_HPRT0_SPEED_LOW		(2)
#define USB_HPRT0_DEFAULT_MASK		(  USB_HPRT0_CONNECT_CHANGED \
						 | USB_HPRT0_ENABLE	   \
						 | USB_HPRT0_ENABLE_CHANGED  \
						 | USB_HPRT0_OVERCURRENT_CHANGED)

void
usb_debug_read()
{
	uart_name_dword("CHECK :  USB_GOTGCTL   = ", IO_READ(USB_GOTGCTL   ));
	uart_name_dword("CHECK :  USB_GINTMSK   = ", IO_READ(USB_GINTMSK   ));
	uart_name_dword("CHECK :  USB_GINTSTS   = ", IO_READ(USB_GINTSTS));
	uart_name_dword("CHECK :  USB_GAHBCFG   = ", IO_READ(USB_GAHBCFG   ));
	uart_name_dword("CHECK :  USB_GUSBCFG   = ", IO_READ(USB_GUSBCFG   ));
	uart_name_dword("CHECK :  USB_GRXFSIZ   = ", IO_READ(USB_GRXFSIZ   ));
	uart_name_dword("CHECK :  USB_GNPTXFSIZ = ", IO_READ(USB_GNPTXFSIZ ));
	uart_name_dword("CHECK :  USB_GDFIFOCFG = ", IO_READ(USB_GDFIFOCFG ));
	uart_name_dword("CHECK :  USB_PCGCCTL1  = ", IO_READ(USB_PCGCCTL1  ));
	uart_name_dword("CHECK :  USB_GLPMCFG   = ", IO_READ(USB_GLPMCFG   ));
	uart_name_dword("CHECK :  USB_GI2CCTL   = ", IO_READ(USB_GI2CCTL   ));
	uart_name_dword("CHECK :  USB_PCGCTL    = ", IO_READ(USB_PCGCTL    ));
	uart_name_dword("USB_GSNPSID(VID) = ", IO_READ(USB_GSNPSID));
	uart_name_dword("USB_GHWCFG1=", IO_READ(USB_GHWCFG1) );
	uart_name_dword("USB_GHWCFG2=", IO_READ(USB_GHWCFG2) );
	uart_name_dword("USB_GHWCFG3=", IO_READ(USB_GHWCFG3) );
	uart_name_dword("USB_GHWCFG4=", IO_READ(USB_GHWCFG4) );
}

void
usb_check_intr()
{
	uint32_t regintr = IO_READ(USB_GINTSTS);
	uint32_t regmask = IO_READ(USB_GINTMSK);
	uart_name_dword("USB_GINTSTS=", IO_READ(USB_GINTSTS) );
	uart_name_dword("USB_GINTMSK=", IO_READ(USB_GINTMSK) );
	if (regintr & USB_GINTSTS_WKUPINT) {
		uart_puts("[INT] USB_GINTSTS_WKUPINT\n");
	}
	if (regintr & USB_GINTSTS_SESSREQINT) {
		uart_puts("[INT] USB_GINTSTS_SESSREQINT\n");
	}
	if (regintr & USB_GINTSTS_DISCONNINT) {
		uart_puts("[INT] USB_GINTSTS_DISCONNINT\n");
	}
	if (regintr & USB_GINTSTS_CONIDSTSCHNG) {
		uart_puts("[INT] USB_GINTSTS_CONIDSTSCH\n");
	}
	if (regintr & USB_GINTSTS_LPMTRANRCVD) {
		uart_puts("[INT] USB_GINTSTS_LPMTRANRCV\n");
	}
	if (regintr & USB_GINTSTS_PTXFEMP) {
		uart_puts("[INT] USB_GINTSTS_PTXFEMP\n");
	}
	if (regintr & USB_GINTSTS_HCHINT) {
		uart_puts("[INT] USB_GINTSTS_HCHINT\n");
	}
	if (regintr & USB_GINTSTS_PRTINT) {
		uart_puts("[INT] USB_GINTSTS_PRTINT\n");
	}
	if (regintr & USB_GINTSTS_RESETDET) {
		uart_puts("[INT] USB_GINTSTS_RESETDET\n");
	}
	if (regintr & USB_GINTSTS_FET_SUSP) {
		uart_puts("[INT] USB_GINTSTS_FET_SUSP\n");
	}
	if (regintr & USB_GINTSTS_INCOMPL_IP) {
		uart_puts("[INT] USB_GINTSTS_INCOMPL_IP\n");
	}
	if (regintr & USB_GINTSTS_INCOMPL_SOOUT) {
		uart_puts("[INT] USB_GINTSTS_INCOMPL_SO\n");
	}
	if (regintr & USB_GINTSTS_INCOMPL_SOIN) {
		uart_puts("[INT] USB_GINTSTS_INCOMPL_SO\n");
	}
	if (regintr & USB_GINTSTS_OEPINT) {
		uart_puts("[INT] USB_GINTSTS_OEPINT\n");
	}
	if (regintr & USB_GINTSTS_IEPINT) {
		uart_puts("[INT] USB_GINTSTS_IEPINT\n");
	}
	if (regintr & USB_GINTSTS_EPMIS) {
		uart_puts("[INT] USB_GINTSTS_EPMIS\n");
	}
	if (regintr & USB_GINTSTS_RESTOREDONE) {
		uart_puts("[INT] USB_GINTSTS_RESTOREDON\n");
	}
	if (regintr & USB_GINTSTS_EOPF) {
		uart_puts("[INT] USB_GINTSTS_EOPF\n");
	}
	if (regintr & USB_GINTSTS_ISOUTDROP) {
		uart_puts("[INT] USB_GINTSTS\n");
	}
	if (regintr & USB_GINTSTS_ENUMDONE) {
		uart_puts("[INT] USB_GINTSTS_ENUMDONE\n");
	}
	if (regintr & USB_GINTSTS_USBRST) {
		uart_puts("[INT] USB_GINTSTS_USBRST\n");
	}
	if (regintr & USB_GINTSTS_USBSUSP) {
		uart_puts("[INT] USB_GINTSTS_USBSUSP\n");
	}
	if (regintr & USB_GINTSTS_ERLYSUSP) {
		uart_puts("[INT] USB_GINTSTS_ERLYSUSP\n");
	}
	if (regintr & USB_GINTSTS_I2CINT) {
		uart_puts("[INT] USB_GINTSTS_I2CINT\n");
	}
	if (regintr & USB_GINTSTS_ULPI_CK_INT) {
		uart_puts("[INT] USB_GINTSTS_ULPI_CK_IN\n");
	}
	if (regintr & USB_GINTSTS_GOUTNAKEFF) {
		uart_puts("[INT] USB_GINTSTS_GOUTNAKEFF\n");
	}
	if (regintr & USB_GINTSTS_GINNAKEFF) {
		uart_puts("[INT] USB_GINTSTS_GINNAKEFF\n");
	}
	if (regintr & USB_GINTSTS_NPTXFEMP) {
		uart_puts("[INT] USB_GINTSTS_NPTXFEMP\n");
	}
	if (regintr & USB_GINTSTS_RXFLVL) {
		uart_puts("[INT] USB_GINTSTS_RXFLVL\n");
	}
	if (regintr & USB_GINTSTS_SOF) {
		uart_puts("[INT] USB_GINTSTS_SOF\n");
	}
	if (regintr & USB_GINTSTS_OTGINT) {
		uart_puts("[INT] USB_GINTSTS_OTGINT\n");
	}
	if (regintr & USB_GINTSTS_MODEMIS) {
		uart_puts("[INT] USB_GINTSTS_MODEMIS\n");
	}
	if (regintr & USB_GINTSTS_CURMODE_HOST) {
		uart_puts("[INT] USB_GINTSTS_CURMODE_HO\n");
	}
	IO_WRITE(USB_GINTSTS, regintr);

}


static void
usb_hw_core_enable_global_int()
{
	uint32_t reg = IO_READ(USB_GAHBCFG);
	reg |= USB_GAHBCFG_EN_INTR;
	IO_WRITE(USB_GAHBCFG, reg);
}


static void
usb_hw_core_disable_global_int()
{
	uint32_t reg = IO_READ(USB_GAHBCFG);
	reg &= ~USB_GAHBCFG_EN_INTR;
	IO_WRITE(USB_GAHBCFG, reg);
}


static void
usb_hw_core_clear_all_int()
{
	IO_WRITE(USB_GOTGINT, 0xFFFFFFFF);
	IO_WRITE(USB_GINTSTS, 0xFFFFFFFF);
}


static void
usb_hw_core_enable_all_int()
{
	uint32_t mask = USB_GINTSTS_CURMODE_HOST | USB_GINTSTS_PTXFEMP | USB_GINTSTS_NPTXFEMP;
	IO_WRITE(USB_GINTMSK, ~mask);
}

static void
usb_hw_core_enable_set_intmask(uint32_t bits)
{
	IO_WRITE(USB_GINTMSK, IO_READ(USB_GINTMSK) | bits);
}

static void
usb_hw_core_disable_intmask()
{
	IO_WRITE(USB_GINTMSK, 0);
}


//https://github.com/torvalds/linux/blob/master/drivers/usb/dwc2/core.c#L853
static void
usb_hw_flush_tx_fifo(int num)
{
	uint32_t reg;
#define USB_GRSTCTL_RXFLUSH  (1 << 5)
#define USB_GRSTCTL_TXFLUSH  (1 << 4)

	//Check Idle State
	while ((IO_READ(USB_GRSTCTL) & USB_GRSTCTL_IDLE) == 0)
		usleep(100000);

	//Send TX Flush
	reg = USB_GRSTCTL_TXFLUSH;
	reg |= (num << 6);
	IO_WRITE(USB_GRSTCTL, reg);

	//Check Flush
	while ((IO_READ(USB_GRSTCTL) & USB_GRSTCTL_TXFLUSH))
		usleep(100);
}

//https://github.com/torvalds/linux/blob/master/drivers/usb/dwc2/core.c#L853
static void
usb_hw_flush_rx_fifo()
{
	uint32_t reg;

	//Check Idle State
	while ((IO_READ(USB_GRSTCTL) & USB_GRSTCTL_IDLE) == 0)
		usleep(100000);

	//Send TX Flush
	reg = USB_GRSTCTL_RXFLUSH;
	IO_WRITE(USB_GRSTCTL, reg);

	//Check Flush
	while ((IO_READ(USB_GRSTCTL) & USB_GRSTCTL_RXFLUSH))
		usleep(100);
}

static void
usb_hw_core_reset()
{
	uart_puts("Check Idle State\n");
	//Check Idle State
	while ((IO_READ(USB_GRSTCTL) & USB_GRSTCTL_IDLE) == 0)
		usleep(100000);

	uart_puts("Issue Reset\n");
	//Reset.
	IO_WRITE(USB_GRSTCTL, IO_READ(USB_GRSTCTL) | USB_GRSTCTL_SOFT_RESET);

	uart_puts("Check Reset Complete\n");
	//Check Reset Complete/
	while (IO_READ(USB_GRSTCTL) & USB_GRSTCTL_SOFT_RESET)
		usleep(1000);

	uart_puts("Check Idle State\n");
	//Check Idle State
	while ((IO_READ(USB_GRSTCTL) & USB_GRSTCTL_IDLE) == 0)
		usleep(100000);
}

#define GAHBCFG_AHB_SINGLE		BIT(23)
#define GAHBCFG_NOTI_ALL_DMA_WRIT	BIT(22)
#define GAHBCFG_REM_MEM_SUPP		BIT(21)
#define GAHBCFG_P_TXF_EMP_LVL		BIT(8)
#define GAHBCFG_NP_TXF_EMP_LVL		BIT(7)
#define GAHBCFG_DMA_EN			BIT(5)
#define GAHBCFG_HBSTLEN_MASK		(0xf << 1)
#define GAHBCFG_HBSTLEN_SHIFT		1
#define GAHBCFG_HBSTLEN_SINGLE		0
#define GAHBCFG_HBSTLEN_INCR		1
#define GAHBCFG_HBSTLEN_INCR4		3
#define GAHBCFG_HBSTLEN_INCR8		5
#define GAHBCFG_HBSTLEN_INCR16		7
#define GAHBCFG_GLBL_INTR_EN		BIT(0)
#define GAHBCFG_CTRL_MASK		(GAHBCFG_P_TXF_EMP_LVL | \
					 GAHBCFG_NP_TXF_EMP_LVL | \
					 GAHBCFG_DMA_EN | \
					 GAHBCFG_GLBL_INTR_EN)

int
usb_hwcfg2_get_hs_phy_type()
{
	#define USB_HS_PHY_TYPE_NOT_SUPPORTED		0
	#define USB_HS_PHY_TYPE_UTMI			1
	#define USB_HS_PHY_TYPE_ULPI			2
	#define USB_HS_PHY_TYPE_UTMI_ULPI		3
	uint32_t reg_hwcfg2 = IO_READ(USB_GHWCFG2);
	reg_hwcfg2 >>= 6;
	reg_hwcfg2 &= 3;
	return reg_hwcfg2;
}

int
usb_hwcfg2_get_fs_phy_type()
{
	#define USB_FS_PHY_TYPE_DEDICATED		1
	uint32_t reg_hwcfg2 = IO_READ(USB_GHWCFG2);
	reg_hwcfg2 >>= 8;
	reg_hwcfg2 &= 3;
	return reg_hwcfg2;
}

int
usb_usbcfg_get_is_ulpi_fsls()
{
	#define USB_USBCFG_ULPI_FSLS		(1 << 17)
	uint32_t reg_usbcfg = IO_READ(USB_GUSBCFG);
	return reg_usbcfg & USB_USBCFG_ULPI_FSLS;
}
typedef union dev_dma_desc_sts {
	/** raw register data */
	uint32_t d32;

	/** quadlet bits */
	struct {
		/** Received number of bytes */
		unsigned bytes:16;
		/** NAK bit - only for OUT EPs */
		unsigned nak:1;
		unsigned reserved17_22:6;
		/** Multiple Transfer - only for OUT EPs */
		unsigned mtrf:1;
		/** Setup Packet received - only for OUT EPs */
		unsigned sr:1;
		/** Interrupt On Complete */
		unsigned ioc:1;
		/** Short Packet */
		unsigned sp:1;
		/** Last */
		unsigned l:1;
		/** Receive Status */
		unsigned sts:2;
		/** Buffer Status */
		unsigned bs:2;
	} b;
} dev_dma_desc_sts_t;


typedef struct dwc_otg_dev_dma_desc {
	/** DMA Descriptor status quadlet */
	dev_dma_desc_sts_t status;
	/** DMA Descriptor data buffer pointer */
	uint32_t buf;
} dwc_otg_dev_dma_desc_t;

static volatile struct dwc_otg_dev_dma_desc usb_desc_ep0 __attribute__ ((aligned (4096)));
static volatile struct USB_DEVICE_REQUEST usb_request __attribute__ ((aligned (4096)));


typedef struct  USB_DEVICE_REQUEST {
  uint8_t           RequestType;
  uint8_t           Request;
  uint16_t          Value;
  uint16_t          Index;
  uint16_t          Length;
} ;

void reset_endpoints(void)
{
  /* EP0 IN ACTIVE NEXT=1 */
  IO_WRITE(USB_DIEPCTL0, 0x8800);

  /* EP0 OUT ACTIVE */
  IO_WRITE(USB_DOEPCTL0, 0x8000);

  /* Clear any pending OTG Interrupts */
  IO_WRITE(USB_GOTGINT, 0xFFFFFFFF);

  /* Clear any pending interrupts */
  IO_WRITE(USB_GINTSTS, 0xFFFFFFFF);
  IO_WRITE(USB_DIEPINT(0), 0xFFFFFFFF);
  IO_WRITE(USB_DOEPINT(0), 0xFFFFFFFF);
  IO_WRITE(USB_DIEPINT(1), 0xFFFFFFFF);
  IO_WRITE(USB_DOEPINT(1), 0xFFFFFFFF);

  /* IN EP interrupt mask */
  IO_WRITE(USB_DIEPMSK, 0x0D);
  /* OUT EP interrupt mask */
  IO_WRITE(USB_DOEPMSK, 0x0D);
  /* Enable interrupts on Ep0 */
  IO_WRITE(USB_DAINTMSK, 0x00010001);

  /* EP0 OUT Transfer Size:64 Bytes, 1 Packet, 3 Setup Packet, Read to receive setup packet*/
  IO_WRITE(USB_DOEPTSIZ0, 0x60080040);

  //notes that:the compulsive conversion is expectable.
  struct dwc_otg_dev_dma_desc *g_dma_usb_desc_ep0 = &usb_desc_ep0;
  g_dma_usb_desc_ep0->status.b.bs = 0x3;
  g_dma_usb_desc_ep0->status.b.mtrf = 0;
  g_dma_usb_desc_ep0->status.b.sr = 0;
  g_dma_usb_desc_ep0->status.b.l = 1;
  g_dma_usb_desc_ep0->status.b.ioc = 1;
  g_dma_usb_desc_ep0->status.b.sp = 0;
  g_dma_usb_desc_ep0->status.b.bytes = 64;
  g_dma_usb_desc_ep0->buf = (uint32_t)(&usb_request);
  g_dma_usb_desc_ep0->status.b.sts = 0;
  g_dma_usb_desc_ep0->status.b.bs = 0x0;
  IO_WRITE(USB_DOEPDMA(0), (unsigned long)(g_dma_usb_desc_ep0));
  /* EP0 OUT ENABLE CLEARNAK */
  IO_WRITE(USB_DOEPCTL0, (IO_READ(USB_DOEPCTL(0)) | 0x84000000));
}


void
usb_host_init()
{
	/*
	asm("dsb  sy");
	asm("isb  sy");
	*/

	//USB power reset
	IO_WRITE(USB_PCGCTL, 0);

	uint32_t reg_hcfg = IO_READ(USB_HCFG);
	reg_hcfg &= ~USB_HCFG_FSLS_PCLK_SEL__MASK;
	
	if(usb_hwcfg2_get_hs_phy_type() == USB_HS_PHY_TYPE_ULPI &&
		usb_hwcfg2_get_fs_phy_type() == USB_FS_PHY_TYPE_DEDICATED &&
		usb_usbcfg_get_is_ulpi_fsls())
	{
		uart_puts("[USB INFO] USB_HCFG_FSLS_PCLK_SEL_48_MHZ\n");
		reg_hcfg |= USB_HCFG_FSLS_PCLK_SEL_48_MHZ;
	} else {
		uart_puts("[USB INFO] USB_HCFG_FSLS_PCLK_SEL_30_60_MHZ\n");
		reg_hcfg |= USB_HCFG_FSLS_PCLK_SEL_30_60_MHZ;
	}
	uart_puts("[USB INFO] SET USB_HCFG\n");
	IO_WRITE(USB_HCFG, reg_hcfg);
	
	//drain fifos.
	uart_puts("[USB INFO] TX FLUSH\n");
	usb_hw_flush_tx_fifo(0x10);
	uart_puts("[USB INFO] RX FLUSH\n");
	usb_hw_flush_rx_fifo();
	uart_puts("[USB INFO] usb_host_init end\n");
	
	/*
	for(int i = 0 ; i < 8; i++) {
		uint32_t reg = IO_READ(USB_HCCHAR(i));
		uart_name_dword("USB_HCCHAR0 : ", reg);
		#define USB_HCCHAR_ENABLE (1 << 31)
		#define USB_HCCHAR_DISABLE (1 << 30)

		reg &= ~USB_HCCHAR_ENABLE;
		reg |=  USB_HCCHAR_DISABLE;
		reg |=  (1 << 15);
		IO_WRITE(USB_HCCHAR(i), reg);
	}
	for(int i = 0 ; i < 8; i++) {
		uint32_t reg = IO_READ(USB_HCCHAR(i));
		uart_name_dword("USB_HCCHAR1 : ", reg);
		reg |=  USB_HCCHAR_ENABLE;
		reg |=  USB_HCCHAR_DISABLE;
		reg |=  (1 << 15);
		IO_WRITE(USB_HCCHAR(i), reg);
	}
	for(int i = 0 ; i < 8; i++) {
		uint32_t reg = IO_READ(USB_HCCHAR(i));
		uart_name_dword("USB_HCCHAR2 : ", reg);
	}
	*/

	//Enable to host power.
	uint32_t reg_hprt0 = IO_READ(USB_HPRT0);
	reg_hprt0 &= ~USB_HPRT0_DEFAULT_MASK;
	if(!(reg_hprt0 & USB_HPRT0_POWER)) {
		reg_hprt0 |= USB_HPRT0_POWER;
		IO_WRITE(USB_HPRT0, reg_hprt0);
	}
	
	//RESET
	IO_WRITE(USB_HPRT0, IO_READ(USB_HPRT0) | (1 << 8));
	usleep(1000000);
	IO_WRITE(USB_HPRT0, IO_READ(USB_HPRT0) & ~(1 << 8));
	usleep(1000000);
	
	//DISABLE all mask
	usb_hw_core_disable_intmask();
	
	//Clear all int
	usb_hw_core_clear_all_int();
	
	//Enable particular ints.
	usb_hw_core_enable_set_intmask(
		USB_GINTSTS_DISCONNINT |
		USB_GINTSTS_PRTINT |
		USB_GINTSTS_HCHINT |
		USB_GINTSTS_SOF);
}


void
usb_hw_config()
{
	uint32_t reg = IO_READ(USB_GAHBCFG);
	reg |= USB_GAHBCFG_EN_DMA;
	reg |= USB_GAHBCFG_WAIT_AXI_WRITES;
	reg &= ~USB_GAHBCFG_MAX_AXI_BURST__MASK;
	IO_WRITE(USB_GAHBCFG, reg);
}

void
usb_hw_core_init()
{
	uint32_t reg_usbcfg = IO_READ(USB_GUSBCFG);
	reg_usbcfg &= ~USB_GUSBCFG_ULPI_EXT_VBUS_DRV;
	reg_usbcfg &= ~USB_GUSBCFG_TERM_SEL_DL_PULSE;
	IO_WRITE(USB_GUSBCFG, reg_usbcfg);
	
	//RESET
	//Check Idle State
	IO_WRITE(USB_GRSTCTL, 0);
	uart_puts("Check Idle State\n");
	while ((IO_READ(USB_GRSTCTL) & USB_GRSTCTL_IDLE) == 0)
		usleep(100000);

	uart_puts("Issue Reset\n");
	//Reset.
	IO_WRITE(USB_GRSTCTL, IO_READ(USB_GRSTCTL) | USB_GRSTCTL_SOFT_RESET);

	uart_puts("Check Reset Complete\n");
	//Check Reset Complete/
	while (IO_READ(USB_GRSTCTL) & USB_GRSTCTL_SOFT_RESET)
		usleep(1000);

	uart_puts("Check Idle State\n");

	//Check Idle State
	while ((IO_READ(USB_GRSTCTL) & USB_GRSTCTL_IDLE) == 0)
		usleep(100000);
	usleep(100000);

	//ENABLE specific status to usbcfg.
	reg_usbcfg = IO_READ(USB_GUSBCFG);
	reg_usbcfg &= ~USB_GUSBCFG_ULPI_UTMI_SEL;
	reg_usbcfg &= ~USB_GUSBCFG_PHYIF;
	IO_WRITE(USB_GUSBCFG, reg_usbcfg);

	if(usb_hwcfg2_get_hs_phy_type() == USB_HS_PHY_TYPE_ULPI &&
		usb_hwcfg2_get_fs_phy_type() == USB_FS_PHY_TYPE_DEDICATED)
	{
		uart_puts("[USB INFO] USB_GUSBCFG_ULPI_FSLS\n");
		uart_puts("[USB INFO] USB_GUSBCFG_ULPI_CLK_SUS_M\n");
		reg_usbcfg |= USB_GUSBCFG_ULPI_FSLS;
		reg_usbcfg |= USB_GUSBCFG_ULPI_CLK_SUS_M;
	} else {
		reg_usbcfg &= ~USB_GUSBCFG_ULPI_FSLS;
		reg_usbcfg &= ~USB_GUSBCFG_ULPI_CLK_SUS_M;
	}
	IO_WRITE(USB_GUSBCFG, reg_usbcfg);

	uint32_t reg_gahbcfg = IO_READ(USB_GAHBCFG);
	reg_gahbcfg |= USB_GAHBCFG_EN_DMA;
	reg_gahbcfg |= USB_GAHBCFG_WAIT_AXI_WRITES;
	reg_gahbcfg &= ~USB_GAHBCFG_MAX_AXI_BURST__MASK;
	IO_WRITE(USB_GAHBCFG, reg_gahbcfg);
	
	reg_usbcfg = IO_READ(USB_GUSBCFG);
	reg_usbcfg &= ~USB_GUSBCFG_SRP_CAPABLE;
	reg_usbcfg &= ~USB_GUSBCFG_HNP_CAPABLE;
	IO_WRITE(USB_GUSBCFG, reg_usbcfg);

	usb_hw_core_clear_all_int();
}

void
usb_hw_init()
{
	//POWER ON
	//todo mb

	uart_name_dword("USB_GSNPSID(VID) = ", IO_READ(USB_GSNPSID));

	//disable global int
	usb_hw_core_disable_global_int();

	//HW core init
	usb_hw_core_init();
	
	//
	usb_hw_core_enable_global_int();

	usb_host_init();

	uart_name_dword("USB_HPRT0 = ", IO_READ(USB_HPRT0));
}





int
usb_init()
{
	int ret = 0;

	usb_hw_init();

	return ret;
}



int
usb_open(int index)
{
	int ret = 0;

	return ret;
}


size_t
usb_read(int index, void *buffer, size_t size)
{
	int ret = -1;


	return ret;
}



size_t
usb_write(int index, void *buffer, size_t size)
{
	int ret = -1;


	return ret;
}

