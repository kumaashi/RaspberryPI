#pragma once

#define GPFSEL3                    0x2020000C
#define GPFSEL4                    0x20200010
#define GPSET1                     0x20200020
#define GPCLR1                     0x2020002C

#define INTR_BASE                  0x2000B000
#define INTR_IRQ_BASIC_PENDING     ((volatile uint32_t *)(INTR_BASE + 0x200))
#define INTR_IRQ_PENDING_1         ((volatile uint32_t *)(INTR_BASE + 0x204))
#define INTR_IRQ_PENDING_2         ((volatile uint32_t *)(INTR_BASE + 0x208))

#define INTR_FIQ_CONTROL           ((volatile uint32_t *)(INTR_BASE + 0x20C))

#define INTR_ENABLE_IRQS_1         ((volatile uint32_t *)(INTR_BASE + 0x210))
#define INTR_ENABLE_IRQS_2         ((volatile uint32_t *)(INTR_BASE + 0x214))
#define INTR_ENABLE_BASIC_IRQS     ((volatile uint32_t *)(INTR_BASE + 0x218))

#define INTR_DISABLE_IRQS_1        ((volatile uint32_t *)(INTR_BASE + 0x21C))
#define INTR_DISABLE_IRQS_2        ((volatile uint32_t *)(INTR_BASE + 0x220))
#define INTR_DISABLE_BASIC_IRQS    ((volatile uint32_t *)(INTR_BASE + 0x224))

#define IRQ_BASIC                  ((volatile uint32_t *)(0x2000B200))
#define IRQ_PEND1                  ((volatile uint32_t *)(0x2000B204))
#define IRQ_ENABLE1                ((volatile uint32_t *)(0x2000B210))

	//uart_debug_puts("INTR_ENABLE_BASIC_IRQS       :", *INTR_ENABLE_BASIC_IRQS   ); 
	//uart_debug_puts("IRQ_BASIC         :", *IRQ_BASIC     ); 

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

#define USB_BASE                   (0x20980000)
#define USB_EX_MDIO_CNTL           ((volatile uint32_t *)(USB_BASE + 0x080))
#define USB_EX_MDIO_DATA           ((volatile uint32_t *)(USB_BASE + 0x084))
#define USB_EX_MDIO_VBUS           ((volatile uint32_t *)(USB_BASE + 0x088))

/* global */
#define USB_GOTGCTL                ((volatile uint32_t *)(USB_BASE + 0x000))
#define USB_GOTGINT                ((volatile uint32_t *)(USB_BASE + 0x004))
#define USB_GAHBCFG                ((volatile uint32_t *)(USB_BASE + 0x008))
#define USB_GUSBCFG                ((volatile uint32_t *)(USB_BASE + 0x00C))
#define USB_GRSTCTL                ((volatile uint32_t *)(USB_BASE + 0x010))
#define USB_GINTSTS                ((volatile uint32_t *)(USB_BASE + 0x014))
#define USB_GINTMSK                ((volatile uint32_t *)(USB_BASE + 0x018))
#define USB_GRXSTSR                ((volatile uint32_t *)(USB_BASE + 0x01C))
#define USB_GRXSTSP                ((volatile uint32_t *)(USB_BASE + 0x020))
#define USB_GRXFSIZ                ((volatile uint32_t *)(USB_BASE + 0x024))
#define USB_GNPTXFSIZ              ((volatile uint32_t *)(USB_BASE + 0x028))
#define USB_GNPTXSTS               ((volatile uint32_t *)(USB_BASE + 0x02C))
#define USB_GI2CCTL                ((volatile uint32_t *)(USB_BASE + 0x0030))
#define USB_GPVNDCTL               ((volatile uint32_t *)(USB_BASE + 0x0034))
#define USB_GGPIO                  ((volatile uint32_t *)(USB_BASE + 0x0038))
#define USB_GUID                   ((volatile uint32_t *)(USB_BASE + 0x003c))
#define USB_GSNPSID                ((volatile uint32_t *)(USB_BASE + 0x0040))
#define USB_GHWCFG1                ((volatile uint32_t *)(USB_BASE + 0x0044))
#define USB_GHWCFG2                ((volatile uint32_t *)(USB_BASE + 0x0048))
#define USB_GHWCFG3                ((volatile uint32_t *)(USB_BASE + 0x004c))
#define USB_GHWCFG4                ((volatile uint32_t *)(USB_BASE + 0x0050))
#define USB_GLPMCFG                ((volatile uint32_t *)(USB_BASE + 0x0054))
#define USB_GPWRDN                 ((volatile uint32_t *)(USB_BASE + 0x0058))
#define USB_GDFIFOCFG              ((volatile uint32_t *)(USB_BASE + 0x005c))
#define USB_GPTXFSIZ               ((volatile uint32_t *)(USB_BASE + 0x0100))
#define USB_DPTXFSIZN(_a)          ((volatile uint32_t *)(USB_BASE + 0x104 + (((_a) - 1) * 4)))
#define USB_DCFG                   ((volatile uint32_t *)(USB_BASE + 0x800))
#define USB_DCTL                   ((volatile uint32_t *)(USB_BASE + 0x804))
#define USB_DSTS                   ((volatile uint32_t *)(USB_BASE + 0x808))
#define USB_DIEPMSK                ((volatile uint32_t *)(USB_BASE + 0x810))
#define USB_DOEPMSK                ((volatile uint32_t *)(USB_BASE + 0x814))
#define USB_DAINT                  ((volatile uint32_t *)(USB_BASE + 0x818))
#define USB_DAINTMSK               ((volatile uint32_t *)(USB_BASE + 0x81C))
#define USB_DTKNQR1                ((volatile uint32_t *)(USB_BASE + 0x820))
#define USB_DTKNQR2                ((volatile uint32_t *)(USB_BASE + 0x824))
#define USB_DTKNQR3                ((volatile uint32_t *)(USB_BASE + 0x830))
#define USB_DTKNQR4                ((volatile uint32_t *)(USB_BASE + 0x834))
#define USB_DIEPEMPMSK             ((volatile uint32_t *)(USB_BASE + 0x834))
#define USB_DVBUSDIS               ((volatile uint32_t *)(USB_BASE + 0x828))
#define USB_DVBUSPULSE             ((volatile uint32_t *)(USB_BASE + 0x82C))
#define USB_DIEPCTL0               ((volatile uint32_t *)(USB_BASE + 0x900))
#define USB_DIEPCTL(_a)            ((volatile uint32_t *)(USB_BASE + 0x900 + ((_a) * 0x20)))
#define USB_DOEPCTL0               ((volatile uint32_t *)(USB_BASE + 0xB00))
#define USB_DOEPCTL(_a)            ((volatile uint32_t *)(USB_BASE + 0xB00 + ((_a) * 0x20)))
#define USB_DIEPINT(_a)            ((volatile uint32_t *)(USB_BASE + 0x908 + ((_a) * 0x20)))
#define USB_DOEPINT(_a)            ((volatile uint32_t *)(USB_BASE + 0xB08 + ((_a) * 0x20)))
#define USB_DIEPTSIZ0              ((volatile uint32_t *)(USB_BASE + 0x910))
#define USB_DOEPTSIZ0              ((volatile uint32_t *)(USB_BASE + 0xB10))
#define USB_DIEPTSIZ(_a)           ((volatile uint32_t *)(USB_BASE + 0x910 + ((_a) * 0x20)))
#define USB_DOEPTSIZ(_a)           ((volatile uint32_t *)(USB_BASE + 0xB10 + ((_a) * 0x20)))
#define USB_DIEPDMA(_a)            ((volatile uint32_t *)(USB_BASE + 0x914 + ((_a) * 0x20)))
#define USB_DOEPDMA(_a)            ((volatile uint32_t *)(USB_BASE + 0xB14 + ((_a) * 0x20)))
#define USB_DTXFSTS(_a)            ((volatile uint32_t *)(USB_BASE + 0x918 + ((_a) * 0x20)))
#define USB_HCFG                   ((volatile uint32_t *)(USB_BASE + 0x0400))
#define USB_HFIR                   ((volatile uint32_t *)(USB_BASE + 0x0404))
#define USB_HFNUM                  ((volatile uint32_t *)(USB_BASE + 0x0408))
#define USB_HPTXSTS                ((volatile uint32_t *)(USB_BASE + 0x0410))
#define USB_HAINT                  ((volatile uint32_t *)(USB_BASE + 0x0414))
#define USB_HAINTMSK               ((volatile uint32_t *)(USB_BASE + 0x0418))
#define USB_HFLBADDR               ((volatile uint32_t *)(USB_BASE + 0x041c))
#define USB_HPRT0                  ((volatile uint32_t *)(USB_BASE + 0x0440))
#define USB_HCCHAR(_ch)            ((volatile uint32_t *)(USB_BASE + 0x0500 + 0x20 * (_ch)))
#define USB_HCSPLT(_ch)            ((volatile uint32_t *)(USB_BASE + 0x0504 + 0x20 * (_ch)))
#define USB_HCINT(_ch)             ((volatile uint32_t *)(USB_BASE + 0x0508 + 0x20 * (_ch)))
#define USB_HCINTMSK(_ch)          ((volatile uint32_t *)(USB_BASE + 0x050c + 0x20 * (_ch)))
#define USB_HCTSIZ(_ch)            ((volatile uint32_t *)(USB_BASE + 0x0510 + 0x20 * (_ch)))
#define USB_HCDMA(_ch)             ((volatile uint32_t *)(USB_BASE + 0x0514 + 0x20 * (_ch)))
#define USB_HCDMAB(_ch)            ((volatile uint32_t *)(USB_BASE + 0x051c + 0x20 * (_ch)))
#define USB_PCGCTL                 ((volatile uint32_t *)(USB_BASE + 0x0e00))
#define USB_PCGCCTL1               ((volatile uint32_t *)(USB_BASE + 0xe04))
#define USB_HCFIFO(_ch)            ((volatile uint32_t *)(USB_BASE + 0x1000 + 0x1000 * (_ch)))
#define USB_EPFIFO(_a)             ((volatile uint32_t *)(USB_BASE + 0x1000 + ((_a) * 0x1000)))


#define GET_FIRMWARE_REVISION 0x00000001 // VideoCore: Get Firmware Revision (Response: Firmware Revision)
#define GET_BOARD_MODEL       0x00010001 // Hardware: Get Board Model (Response: Board Model)
#define GET_BOARD_REVISION    0x00010002 // Hardware: Get Board Revision (Response: Board Revision)
#define GET_BOARD_MAC_ADDRESS 0x00010003 // Hardware: Get Board MAC Address (Response: MAC Address In Network Byte Order)
#define GET_BOARD_SERIAL      0x00010004 // Hardware: Get Board Serial (Response: Board Serial)
#define GET_ARM_MEMORY        0x00010005 // Hardware: Get ARM Memory (Response: Base Address In Bytes, Size In Bytes)
#define GET_VC_MEMORY         0x00010006 // Hardware: Get VC Memory (Response: Base Address In Bytes, Size In Bytes)
#define GET_CLOCKS            0x00010007 // Hardware: Get Clocks (Response: Parent Clock ID (0 For A Root Clock), Clock ID)
#define GET_POWER_STATE       0x00020001 // Power: Get Power State (Response: Device ID, State)
#define GET_TIMING            0x00020002 // Power: Get Timing (Response: Device ID, Enable Wait Time In Microseconds)
#define SET_POWER_STATE       0x00028001 // Power: Set Power State (Response: Device ID, State)
#define GET_CLOCK_STATE       0x00030001 // Clocks: Get Clock State (Response: Clock ID, State)
#define GET_CLOCK_RATE        0x00030002 // Clocks: Get Clock Rate (Response: Clock ID, Rate In Hz)
#define GET_VOLTAGE           0x00030003 // Voltage: Get Voltage (Response: Voltage ID, Value Offset From 1.2V In Units Of 0.025V)
#define GET_MAX_CLOCK_RATE    0x00030004 // Clocks: Get Max Clock Rate (Response: Clock ID, Rate In Hz)
#define GET_MAX_VOLTAGE       0x00030005 // Voltage: Get Max Voltage (Response: Voltage ID, Value Offset From 1.2V In Units Of 0.025V)
#define GET_TEMPERATURE       0x00030006 // Voltage: Get Temperature (Response: Temperature ID, Value In Degrees C)
#define GET_MIN_CLOCK_RATE    0x00030007 // Clocks: Get Min Clock Rate (Response: Clock ID, Rate In Hz)
#define GET_MIN_VOLTAGE       0x00030008 // Voltage: Get Min Voltage (Response: Voltage ID, Value Offset From 1.2V In Units Of 0.025V)
#define GET_TURBO             0x00030009 // Clocks: Get Turbo (Response: ID, Level)
#define GET_MAX_TEMPERATURE   0x0003000A // Voltage: Get Max Temperature (Response: Temperature ID, Value In Degrees C)
#define ALLOCATE_MEMORY       0x0003000C // Memory: Allocates Contiguous Memory On The GPU (Response: Handle)
#define LOCK_MEMORY           0x0003000D // Memory: Lock Buffer In Place, And Return A Bus Address (Response: Bus Address)
#define UNLOCK_MEMORY         0x0003000E // Memory: Unlock Buffer (Response: Status)
#define RELEASE_MEMORY        0x0003000F // Memory: Free The Memory Buffer (Response: Status)
#define EXECUTE_CODE          0x00030010 // Memory: Calls The Function At Given (Bus) Address And With Arguments Given
#define EXECUTE_QPU           0x00030011 // QPU: Calls The QPU Function At Given (Bus) Address And With Arguments Given (Response: Number Of QPUs, Control, No Flush, Timeout In ms)
#define ENABLE_QPU            0x00030012 // QPU: Enables The QPU (Response: Enable State)
#define GET_EDID_BLOCK        0x00030020 // HDMI: Read Specificed EDID Block From Attached HDMI/DVI Device (Response: Block Number, Status, EDID Block (128 Bytes))
#define SET_CLOCK_STATE       0x00038001 // Clocks: Set Clock State (Response: Clock ID, State)
#define SET_CLOCK_RATE        0x00038002 // Clocks: Set Clock Rate (Response: Clock ID, Rate In Hz)
#define SET_VOLTAGE           0x00038003 // Voltage: Set Voltage (Response: Voltage ID, Value Offset From 1.2V In Units Of 0.025V)
#define GET_DOMAIN_STATE      0x00030030
#define SET_DOMAIN_STATE      0x00038030
#define NOTIFY_REBOOT         0x00030048
#define SET_TURBO             0x00038009 // Clocks: Set Turbo (Response: ID, Level)
#define ALLOCATE_BUFFER       0x00040001 // Frame Buffer: Allocate Buffer (Response: Frame Buffer Base Address In Bytes, Frame Buffer Size In Bytes)
#define BLANK_SCREEN          0x00040002 // Frame Buffer: Blank Screen (Response: State)
#define GET_PHYSICAL_DISPLAY  0x00040003 // Frame Buffer: Get Physical (Display) Width/Height (Response: Width In Pixels, Height In Pixels)
#define GET_VIRTUAL_BUFFER    0x00040004 // Frame Buffer: Get Virtual (Buffer) Width/Height (Response: Width In Pixels, Height In Pixels)
#define GET_DEPTH             0x00040005 // Frame Buffer: Get Depth (Response: Bits Per Pixel)
#define GET_PIXEL_ORDER       0x00040006 // Frame Buffer: Get Pixel Order (Response: State)
#define GET_ALPHA_MODE        0x00040007 // Frame Buffer: Get Alpha Mode (Response: State)
#define GET_PITCH             0x00040008 // Frame Buffer: Get Pitch (Response: Bytes Per Line)
#define GET_VIRTUAL_OFFSET    0x00040009 // Frame Buffer: Get Virtual Offset (Response: X In Pixels, Y In Pixels)
#define GET_OVERSCAN          0x0004000A // Frame Buffer: Get Overscan (Response: Top In Pixels, Bottom In Pixels, Left In Pixels, Right In Pixels)
#define GET_PALETTE           0x0004000B // Frame Buffer: Get Palette (Response: RGBA Palette Values (Index 0 To 255))
#define TEST_PHYSICAL_DISPLAY 0x00044003 // Frame Buffer: Test Physical (Display) Width/Height (Response: Width In Pixels, Height In Pixels)
#define TEST_VIRTUAL_BUFFER   0x00044004 // Frame Buffer: Test Virtual (Buffer) Width/Height (Response: Width In Pixels, Height In Pixels)
#define TEST_DEPTH            0x00044005 // Frame Buffer: Test Depth (Response: Bits Per Pixel)
#define TEST_PIXEL_ORDER      0x00044006 // Frame Buffer: Test Pixel Order (Response: State)
#define TEST_ALPHA_MODE       0x00044007 // Frame Buffer: Test Alpha Mode (Response: State)
#define TEST_VIRTUAL_OFFSET   0x00044009 // Frame Buffer: Test Virtual Offset (Response: X In Pixels, Y In Pixels)
#define TEST_OVERSCAN         0x0004400A // Frame Buffer: Test Overscan (Response: Top In Pixels, Bottom In Pixels, Left In Pixels, Right In Pixels)
#define TEST_PALETTE          0x0004400B // Frame Buffer: Test Palette (Response: RGBA Palette Values (Index 0 To 255))
#define RELEASE_BUFFER        0x00048001 // Frame Buffer: Release Buffer (Response: Releases And Disables The Frame Buffer)
#define SET_PHYSICAL_DISPLAY  0x00048003 // Frame Buffer: Set Physical (Display) Width/Height (Response: Width In Pixels, Height In Pixels)
#define SET_VIRTUAL_BUFFER    0x00048004 // Frame Buffer: Set Virtual (Buffer) Width/Height (Response: Width In Pixels, Height In Pixels)
#define SET_DEPTH             0x00048005 // Frame Buffer: Set Depth (Response: Bits Per Pixel)
#define SET_PIXEL_ORDER       0x00048006 // Frame Buffer: Set Pixel Order (Response: State)
#define SET_ALPHA_MODE        0x00048007 // Frame Buffer: Set Alpha Mode (Response: State)
#define SET_VIRTUAL_OFFSET    0x00048009 // Frame Buffer: Set Virtual Offset (Response: X In Pixels, Y In Pixels)
#define SET_OVERSCAN          0x0004800A // Frame Buffer: Set Overscan (Response: Top In Pixels, Bottom In Pixels, Left In Pixels, Right In Pixels)
#define SET_PALETTE           0x0004800B // Frame Buffer: Set Palette (Response: RGBA Palette Values (Index 0 To 255))
#define GET_COMMAND_LINE      0x00050001 // Config: Get Command Line (Response: ASCII Command Line String)
#define GET_DMA_CHANNELS      0x00060001 // Shared Resource Management: Get DMA Channels (Response: Bits 0-15: DMA channels 0-15)

