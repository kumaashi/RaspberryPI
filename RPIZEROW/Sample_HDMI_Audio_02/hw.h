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
#define USB_HPRT0_HCINT            (1 << 25)
#define USB_HPRT0_PRTINT           (1 << 24)
#define USB_HPRT0_INCOMPLP         (1 << 21)


#define HD_BASE                    (0x20808000)
#define HDMI_M_CTL                 ((volatile uint32_t *)(HD_BASE + 0x000c))
#define HDMI_MAI_CTL               ((volatile uint32_t *)(HD_BASE + 0x0014))
#define HDMI_MAI_THR               ((volatile uint32_t *)(HD_BASE + 0x0018))
#define HDMI_MAI_FMT               ((volatile uint32_t *)(HD_BASE + 0x001c))
#define HDMI_MAI_DATA              ((volatile uint32_t *)(HD_BASE + 0x0020))
#define HDMI_MAI_DATA1             ((volatile uint32_t *)(HD_BASE + 0x0024))
#define HDMI_MAI_SMP               ((volatile uint32_t *)(HD_BASE + 0x002c))
#define HDMI_VID_CTL               ((volatile uint32_t *)(HD_BASE + 0x0038))
#define HDMI_CSC_CTL               ((volatile uint32_t *)(HD_BASE + 0x0040))
#define HDMI_CSC_12_11             ((volatile uint32_t *)(HD_BASE + 0x0044))
#define HDMI_CSC_14_13             ((volatile uint32_t *)(HD_BASE + 0x0048))
#define HDMI_CSC_22_21             ((volatile uint32_t *)(HD_BASE + 0x004c))
#define HDMI_CSC_24_23             ((volatile uint32_t *)(HD_BASE + 0x0050))
#define HDMI_CSC_32_31             ((volatile uint32_t *)(HD_BASE + 0x0054))
#define HDMI_CSC_34_33             ((volatile uint32_t *)(HD_BASE + 0x0058))
#define HDMI_FRAME_COUNT           ((volatile uint32_t *)(HD_BASE + 0x0068))


#define HDMI_BASE                  (0x20902000)
#define HDMI_CORE_REV              ((volatile uint32_t *)(HDMI_BASE + 0x0000))
#define HDMI_SW_RESET_CONTROL      ((volatile uint32_t *)(HDMI_BASE + 0x0004))
#define HDMI_HOTPLUG_INT           ((volatile uint32_t *)(HDMI_BASE + 0x0008))
#define HDMI_HOTPLUG               ((volatile uint32_t *)(HDMI_BASE + 0x000c))
#define HDMI_FIFO_CTL              ((volatile uint32_t *)(HDMI_BASE + 0x005c))
#define HDMI_MAI_CHANNEL_MAP       ((volatile uint32_t *)(HDMI_BASE + 0x0090))
#define HDMI_MAI_CONFIG            ((volatile uint32_t *)(HDMI_BASE + 0x0094))
#define HDMI_MAI_FORMAT            ((volatile uint32_t *)(HDMI_BASE + 0x0098))
#define HDMI_AUDIO_PACKET_CONFIG   ((volatile uint32_t *)(HDMI_BASE + 0x009c))
#define HDMI_RAM_PACKET_CONFIG     ((volatile uint32_t *)(HDMI_BASE + 0x00a0))
#define HDMI_RAM_PACKET_STATUS     ((volatile uint32_t *)(HDMI_BASE + 0x00a4))
#define HDMI_CRP_CFG               ((volatile uint32_t *)(HDMI_BASE + 0x00a8))
#define HDMI_CTS_0                 ((volatile uint32_t *)(HDMI_BASE + 0x00ac))
#define HDMI_CTS_1                 ((volatile uint32_t *)(HDMI_BASE + 0x00b0))
#define HDMI_SCHEDULER_CONTROL     ((volatile uint32_t *)(HDMI_BASE + 0x00c0))
#define HDMI_HORZA                 ((volatile uint32_t *)(HDMI_BASE + 0x00c4))
#define HDMI_HORZB                 ((volatile uint32_t *)(HDMI_BASE + 0x00c8))
#define HDMI_VERTA0                ((volatile uint32_t *)(HDMI_BASE + 0x00cc))
#define HDMI_VERTB0                ((volatile uint32_t *)(HDMI_BASE + 0x00d0))
#define HDMI_VERTA1                ((volatile uint32_t *)(HDMI_BASE + 0x00d4))
#define HDMI_VERTB1                ((volatile uint32_t *)(HDMI_BASE + 0x00d8))
#define HDMI_CEC_CNTRL_1           ((volatile uint32_t *)(HDMI_BASE + 0x00e8))
#define HDMI_CEC_CNTRL_2           ((volatile uint32_t *)(HDMI_BASE + 0x00ec))
#define HDMI_CEC_CNTRL_3           ((volatile uint32_t *)(HDMI_BASE + 0x00f0))
#define HDMI_CEC_CNTRL_4           ((volatile uint32_t *)(HDMI_BASE + 0x00f4))
#define HDMI_CEC_CNTRL_5           ((volatile uint32_t *)(HDMI_BASE + 0x00f8))
#define HDMI_CEC_TX_DATA_1         ((volatile uint32_t *)(HDMI_BASE + 0x00fc))
#define HDMI_CEC_TX_DATA_2         ((volatile uint32_t *)(HDMI_BASE + 0x0100))
#define HDMI_CEC_TX_DATA_3         ((volatile uint32_t *)(HDMI_BASE + 0x0104))
#define HDMI_CEC_TX_DATA_4         ((volatile uint32_t *)(HDMI_BASE + 0x0108))
#define HDMI_CEC_RX_DATA_1         ((volatile uint32_t *)(HDMI_BASE + 0x010c))
#define HDMI_CEC_RX_DATA_2         ((volatile uint32_t *)(HDMI_BASE + 0x0110))
#define HDMI_CEC_RX_DATA_3         ((volatile uint32_t *)(HDMI_BASE + 0x0114))
#define HDMI_CEC_RX_DATA_4         ((volatile uint32_t *)(HDMI_BASE + 0x0118))
#define HDMI_TX_PHY_RESET_CTL      ((volatile uint32_t *)(HDMI_BASE + 0x02c0))
#define HDMI_TX_PHY_CTL_0          ((volatile uint32_t *)(HDMI_BASE + 0x02c4))
#define HDMI_CEC_CPU_STATUS        ((volatile uint32_t *)(HDMI_BASE + 0x0340))
#define HDMI_CEC_CPU_SET           ((volatile uint32_t *)(HDMI_BASE + 0x0344))
#define HDMI_CEC_CPU_CLEAR         ((volatile uint32_t *)(HDMI_BASE + 0x0348))
#define HDMI_CEC_CPU_MASK_STATUS   ((volatile uint32_t *)(HDMI_BASE + 0x034c))
#define HDMI_CEC_CPU_MASK_SET      ((volatile uint32_t *)(HDMI_BASE + 0x0350))
#define HDMI_CEC_CPU_MASK_CLEAR    ((volatile uint32_t *)(HDMI_BASE + 0x0354))
#define HDMI_RAM_PACKET_START      ((volatile uint32_t *)(HDMI_BASE + 0x0400))
#define HDMI_RAM_PACKET(x, i)      ((volatile uint32_t *)(HDMI_BASE + 0x0400 + (0x24 * (x) + (0x4 * (i)))))













