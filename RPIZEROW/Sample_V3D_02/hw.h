#pragma once

#define GPIO_BASE                  (SUBSYSTEM_BASE + 0x200000)
#define GPFSEL3                    (GPIO_BASE + 0x0C)
#define GPFSEL4                    (GPIO_BASE + 0x10)
#define GPSET1                     (GPIO_BASE + 0x20)
#define GPCLR1                     (GPIO_BASE + 0x2C)

#define D_GPFSEL3                  ((volatile uint32_t *)(GPFSEL3))
#define D_GPFSEL4                  ((volatile uint32_t *)(GPFSEL4))
#define D_GPSET1                   ((volatile uint32_t *)(GPSET1 ))
#define D_GPCLR1                   ((volatile uint32_t *)(GPCLR1 ))

#define INTR_BASE                  (SUBSYSTEM_BASE + 0xB000)
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

//https://github.com/raspberrypi/firmware/issues/67
#define IRQ_BASE                   (SUBSYSTEM_BASE + 0xB200)
#define IRQ_GPU_PENDING1           ((volatile uint32_t *)(SUBSYSTEM_BASE + 0xB204))
#define IRQ_GPU_PENDING2           ((volatile uint32_t *)(SUBSYSTEM_BASE + 0xB208))
#define IRQ_GPU_ENABLE2            ((volatile uint32_t *)(SUBSYSTEM_BASE + 0xB214))
#define IRQ_GPU_FAKE_ISR           0x10000

#define SMI_BASE                   (SUBSYSTEM_BASE + 0x600000)
#define SMI_CS                     ((volatile uint32_t *)(SMI_BASE + 0x00))

#define SYST_BASE                  (SUBSYSTEM_BASE + 0x3000)
#define SYST_CLO                   ((volatile uint32_t *)(SYST_BASE + 0x04))
#define SYST_CHI                   ((volatile uint32_t *)(SYST_BASE + 0x08))

#define TIMER_BASE                 (SUBSYSTEM_BASE + 0xB000)
#define TIMER_LOAD                 (TIMER_BASE + 0x400) // Load
#define TIMER_VALUE                (TIMER_BASE + 0x404) // Value (Read Only)
#define TIMER_CONTROL              (TIMER_BASE + 0x408) // Control
#define TIMER_IRQ_CLEAR_ACK        (TIMER_BASE + 0x40C) // IRQ Clear/Ack (Write only)
#define TIMER_RAW_IRQ              (TIMER_BASE + 0x410) // RAW IRQ (Read Only)
#define TIMER_MASKED_IRQ           (TIMER_BASE + 0x414) // Masked IRQ (Read Only)
#define TIMER_RELOAD               (TIMER_BASE + 0x418) // Reload
#define TIMER_PRE_DIVIDER          (TIMER_BASE + 0x41C) // Pre-divider (Not in real 804!)
#define TIMER_FREE_RUNNING_COUNTER (TIMER_BASE + 0x420) // Free running counter (Not in real 804!)

#define USB_BASE                   (SUBSYSTEM_BASE + 0x980000)
#define USB_EX_MDIO_CNTL           ((volatile uint32_t *)(USB_BASE + 0x080))
#define USB_EX_MDIO_DATA           ((volatile uint32_t *)(USB_BASE + 0x084))
#define USB_EX_MDIO_VBUS           ((volatile uint32_t *)(USB_BASE + 0x088))

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


#define HD_BASE                    (SUBSYSTEM_BASE + 0x808000)
#define HD_BUS_BASE                (0x7E808000)
#define HDMI_M_CTL                 ((volatile uint32_t *)(HD_BASE + 0x000c))
#define HDMI_MAI_CTL               ((volatile uint32_t *)(HD_BASE + 0x0014))
#define HDMI_MAI_THR               ((volatile uint32_t *)(HD_BASE + 0x0018))
#define HDMI_MAI_FMT               ((volatile uint32_t *)(HD_BASE + 0x001c))
#define HDMI_MAI_DATA              ((volatile uint32_t *)(HD_BASE + 0x0020))
#define HDMI_MAI_DATA_BUS          ((volatile uint32_t *)(HD_BUS_BASE + 0x0020))
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

#define HDMI_BASE                  (SUBSYSTEM_BASE + 0x902000)
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


#define V3D_BASE                   (SUBSYSTEM_BASE + 0xC00000)
#define V3D_IDENT0                 ((volatile uint32_t *)(V3D_BASE + (0x000))) // V3D Identification 0 (V3D block identity)
#define V3D_IDENT1                 ((volatile uint32_t *)(V3D_BASE + (0x004))) // V3D Identification 1 (V3D Configuration A)
#define V3D_IDENT2                 ((volatile uint32_t *)(V3D_BASE + (0x008))) // V3D Identification 2 (V3D Configuration B)
#define V3D_SCRATCH                ((volatile uint32_t *)(V3D_BASE + (0x010))) // Scratch Register
#define V3D_L2CACTL                ((volatile uint32_t *)(V3D_BASE + (0x020))) // 2 Cache Control
#define V3D_SLCACTL                ((volatile uint32_t *)(V3D_BASE + (0x024))) // Slices Cache Control
#define V3D_INTCTL                 ((volatile uint32_t *)(V3D_BASE + (0x030))) // Interrupt Control
#define V3D_INTENA                 ((volatile uint32_t *)(V3D_BASE + (0x034))) // Interrupt Enables
#define V3D_INTDIS                 ((volatile uint32_t *)(V3D_BASE + (0x038))) // Interrupt Disables
#define V3D_CT0CS                  ((volatile uint32_t *)(V3D_BASE + (0x100))) // Control List Executor Thread 0 Control and Status.
#define V3D_CT1CS                  ((volatile uint32_t *)(V3D_BASE + (0x104))) // Control List Executor Thread 1 Control and Status.
#define V3D_CT0EA                  ((volatile uint32_t *)(V3D_BASE + (0x108))) // Control List Executor Thread 0 End Address.
#define V3D_CT1EA                  ((volatile uint32_t *)(V3D_BASE + (0x10c))) // Control List Executor Thread 1 End Address.
#define V3D_CT0CA                  ((volatile uint32_t *)(V3D_BASE + (0x110))) // Control List Executor Thread 0 Current Address.
#define V3D_CT1CA                  ((volatile uint32_t *)(V3D_BASE + (0x114))) // Control List Executor Thread 1 Current Address.
#define V3D_CT00RA0                ((volatile uint32_t *)(V3D_BASE + (0x118))) // Control List Executor Thread 0 Return Address.
#define V3D_CT01RA0                ((volatile uint32_t *)(V3D_BASE + (0x11c))) // Control List Executor Thread 1 Return Address.
#define V3D_CT0LC                  ((volatile uint32_t *)(V3D_BASE + (0x120))) // Control List Executor Thread 0 List Counter
#define V3D_CT1LC                  ((volatile uint32_t *)(V3D_BASE + (0x124))) // Control List Executor Thread 1 List Counter
#define V3D_CT0PC                  ((volatile uint32_t *)(V3D_BASE + (0x128))) // Control List Executor Thread 0 Primitive List Counter
#define V3D_CT1PC                  ((volatile uint32_t *)(V3D_BASE + (0x12c))) // Control List Executor Thread 1 Primitive List Counter
#define V3D_PCS                    ((volatile uint32_t *)(V3D_BASE + (0x130))) // V3D Pipeline Control and Status
#define V3D_BFC                    ((volatile uint32_t *)(V3D_BASE + (0x134))) // Binning Mode Flush Count
#define V3D_RFC                    ((volatile uint32_t *)(V3D_BASE + (0x138))) // Rendering Mode Frame Count
#define V3D_BPCA                   ((volatile uint32_t *)(V3D_BASE + (0x300))) // Current Address of Binning Memory Pool
#define V3D_BPCS                   ((volatile uint32_t *)(V3D_BASE + (0x304))) // Remaining Size of Binning Memory Pool
#define V3D_BPOA                   ((volatile uint32_t *)(V3D_BASE + (0x308))) // Address of Overspill Binning Memory Block
#define V3D_BPOS                   ((volatile uint32_t *)(V3D_BASE + (0x30c))) // Size of Overspill Binning Memory Block
#define V3D_BXCF                   ((volatile uint32_t *)(V3D_BASE + (0x310))) // Binner Debug
#define V3D_SQRSV0                 ((volatile uint32_t *)(V3D_BASE + (0x410))) // Reserve QPUs 0-7
#define V3D_SQRSV1                 ((volatile uint32_t *)(V3D_BASE + (0x414))) // Reserve QPUs 8-15
#define V3D_SQCNTL                 ((volatile uint32_t *)(V3D_BASE + (0x418))) // QPU Scheduler Control
#define V3D_SRQPC                  ((volatile uint32_t *)(V3D_BASE + (0x430))) // QPU User Program Request Program Address
#define V3D_SRQUA                  ((volatile uint32_t *)(V3D_BASE + (0x434))) // QPU User Program Request Uniforms Address
#define V3D_SRQUL                  ((volatile uint32_t *)(V3D_BASE + (0x438))) // QPU User Program Request Uniforms Length
#define V3D_SRQCS                  ((volatile uint32_t *)(V3D_BASE + (0x43c))) // QPU User Program Request Control and Status
#define V3D_VPACNTL                ((volatile uint32_t *)(V3D_BASE + (0x500))) // VPM Allocator Control
#define V3D_VPMBASE                ((volatile uint32_t *)(V3D_BASE + (0x504))) // VPM base (user) memory reservation
#define V3D_PCTRC                  ((volatile uint32_t *)(V3D_BASE + (0x670))) // Performance Counter Clear
#define V3D_PCTRE                  ((volatile uint32_t *)(V3D_BASE + (0x674))) // Performance Counter Enables
#define V3D_PCTR0                  ((volatile uint32_t *)(V3D_BASE + (0x680))) // Performance Counter Count 0
#define V3D_PCTRS0                 ((volatile uint32_t *)(V3D_BASE + (0x684))) // Performance Counter Mapping 0
#define V3D_PCTR1                  ((volatile uint32_t *)(V3D_BASE + (0x688))) // Performance Counter Count 1
#define V3D_PCTRS1                 ((volatile uint32_t *)(V3D_BASE + (0x68c))) // Performance Counter Mapping 1
#define V3D_PCTR2                  ((volatile uint32_t *)(V3D_BASE + (0x690))) // Performance Counter Count 2
#define V3D_PCTRS2                 ((volatile uint32_t *)(V3D_BASE + (0x694))) // Performance Counter Mapping 2
#define V3D_PCTR3                  ((volatile uint32_t *)(V3D_BASE + (0x698))) // Performance Counter Count 3
#define V3D_PCTRS3                 ((volatile uint32_t *)(V3D_BASE + (0x69c))) // Performance Counter Mapping 3
#define V3D_PCTR4                  ((volatile uint32_t *)(V3D_BASE + (0x6a0))) // Performance Counter Count 4
#define V3D_PCTRS4                 ((volatile uint32_t *)(V3D_BASE + (0x6a4))) // Performance Counter Mapping 4
#define V3D_PCTR5                  ((volatile uint32_t *)(V3D_BASE + (0x6a8))) // Performance Counter Count 5
#define V3D_PCTRS5                 ((volatile uint32_t *)(V3D_BASE + (0x6ac))) // Performance Counter Mapping 5
#define V3D_PCTR6                  ((volatile uint32_t *)(V3D_BASE + (0x6b0))) // Performance Counter Count 6
#define V3D_PCTRS6                 ((volatile uint32_t *)(V3D_BASE + (0x6b4))) // Performance Counter Mapping 6
#define V3D_PCTR7                  ((volatile uint32_t *)(V3D_BASE + (0x6b8))) // Performance Counter Count 7
#define V3D_PCTRS7                 ((volatile uint32_t *)(V3D_BASE + (0x6bc))) // Performance Counter Mapping 7 
#define V3D_PCTR8                  ((volatile uint32_t *)(V3D_BASE + (0x6c0))) // Performance Counter Count 8
#define V3D_PCTRS8                 ((volatile uint32_t *)(V3D_BASE + (0x6c4))) // Performance Counter Mapping 8
#define V3D_PCTR9                  ((volatile uint32_t *)(V3D_BASE + (0x6c8))) // Performance Counter Count 9
#define V3D_PCTRS9                 ((volatile uint32_t *)(V3D_BASE + (0x6cc))) // Performance Counter Mapping 9
#define V3D_PCTR10                 ((volatile uint32_t *)(V3D_BASE + (0x6d0))) // Performance Counter Count 10
#define V3D_PCTRS10                ((volatile uint32_t *)(V3D_BASE + (0x6d4))) // Performance Counter Mapping 10
#define V3D_PCTR11                 ((volatile uint32_t *)(V3D_BASE + (0x6d8))) // Performance Counter Count 11
#define V3D_PCTRS11                ((volatile uint32_t *)(V3D_BASE + (0x6dc))) // Performance Counter Mapping 11
#define V3D_PCTR12                 ((volatile uint32_t *)(V3D_BASE + (0x6e0))) // Performance Counter Count 12
#define V3D_PCTRS12                ((volatile uint32_t *)(V3D_BASE + (0x6e4))) // Performance Counter Mapping 12
#define V3D_PCTR13                 ((volatile uint32_t *)(V3D_BASE + (0x6e8))) // Performance Counter Count 13
#define V3D_PCTRS13                ((volatile uint32_t *)(V3D_BASE + (0x6ec))) // Performance Counter Mapping 13
#define V3D_PCTR14                 ((volatile uint32_t *)(V3D_BASE + (0x6f0))) // Performance Counter Count 14
#define V3D_PCTRS14                ((volatile uint32_t *)(V3D_BASE + (0x6f4))) // Performance Counter Mapping 14
#define V3D_PCTR15                 ((volatile uint32_t *)(V3D_BASE + (0x6f8))) // Performance Counter Count 15
#define V3D_PCTRS15                ((volatile uint32_t *)(V3D_BASE + (0x6fc))) // Performance Counter Mapping 15
#define V3D_DBGE                   ((volatile uint32_t *)(V3D_BASE + (0xf00))) // PSE Error Signals
#define V3D_FDBGO                  ((volatile uint32_t *)(V3D_BASE + (0xf04))) // FEP Overrun Error Signals
#define V3D_FDBGB                  ((volatile uint32_t *)(V3D_BASE + (0xf08))) // FEP Interface Ready and Stall Signals, FEP Busy Signals
#define V3D_FDBGR                  ((volatile uint32_t *)(V3D_BASE + (0xf0c))) // FEP Internal Ready Signals
#define V3D_FDBGS                  ((volatile uint32_t *)(V3D_BASE + (0xf10))) // FEP Internal Stall Input Signals
#define V3D_ERRSTAT                ((volatile uint32_t *)(V3D_BASE + (0xf20))) // Miscellaneous Error Signals (VPM, VDW, VCD, VCM, L2C)

