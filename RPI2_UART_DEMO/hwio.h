#ifndef _HWIO_H_
#define _HWIO_H_

#include <stdint.h>

#define IO_READ(reg)                                          (*(volatile uint32_t *)((uint32_t)(reg)))
#define IO_WRITE(reg, val)                                    (*(volatile uint32_t *)((uint32_t)(reg)) = (uint32_t)val)

#define PM_PASSWORD                                           0x5A000000 

//#define SUBSYSTEM_BASE                                        0x20000000  //PI1
#define SUBSYSTEM_BASE                                        0x3F000000  //PI2
#define GPIO_BASE                                             (SUBSYSTEM_BASE + 0x200000)
#define GPIO_SEL0                                             (GPIO_BASE + (4 * 0))
#define GPIO_SEL1                                             (GPIO_BASE + (4 * 1))
#define GPIO_SEL2                                             (GPIO_BASE + (4 * 2))
#define GPIO_SEL3                                             (GPIO_BASE + (4 * 3))
#define GPIO_SEL4                                             (GPIO_BASE + (4 * 4))
#define GPIO_SEL5                                             (GPIO_BASE + (4 * 5))
#define GPIO_SET0                                             (GPIO_BASE + 0x1C)
#define GPIO_SET1                                             (GPIO_BASE + 0x20)
#define GPIO_CLR0                                             (GPIO_BASE + 0x28)
#define GPIO_CLR1                                             (GPIO_BASE + 0x2C)
#define GPIO_LEV0                                             (GPIO_BASE + 0x34)
#define GPIO_LEV1                                             (GPIO_BASE + 0x38)
#define GPIO_GPPUD                                            (GPIO_BASE + 0x94)
#define GPIO_GPPUDCLK0                                        (GPIO_BASE + 0x98)
#define GPIO_INPUT                                            (0)
#define GPIO_OUTPUT                                           (1)
#define GPIO_ALT0                                             (4)
#define GPIO_ALT1                                             (5)
#define GPIO_FSEL0_IN                                         0x0 // GPIO Function Select: GPIO Pin X0 Is An Input
#define GPIO_FSEL0_OUT                                        0x1 // GPIO Function Select: GPIO Pin X0 Is An Output
#define GPIO_FSEL0_ALT0                                       0x4 // GPIO Function Select: GPIO Pin X0 Takes Alternate Function 0
#define GPIO_FSEL0_ALT1                                       0x5 // GPIO Function Select: GPIO Pin X0 Takes Alternate Function 1
#define GPIO_FSEL0_ALT2                                       0x6 // GPIO Function Select: GPIO Pin X0 Takes Alternate Function 2
#define GPIO_FSEL0_ALT3                                       0x7 // GPIO Function Select: GPIO Pin X0 Takes Alternate Function 3
#define GPIO_FSEL0_ALT4                                       0x3 // GPIO Function Select: GPIO Pin X0 Takes Alternate Function 4
#define GPIO_FSEL0_ALT5                                       0x2 // GPIO Function Select: GPIO Pin X0 Takes Alternate Function 5
#define GPIO_FSEL0_CLR                                        0x7 // GPIO Function Select: GPIO Pin X0 Clear Bits
#define GPIO_FSEL1_IN                                          0x0 // GPIO Function Select: GPIO Pin X1 Is An Input
#define GPIO_FSEL1_OUT                                         0x8 // GPIO Function Select: GPIO Pin X1 Is An Output
#define GPIO_FSEL1_ALT0                                       0x20 // GPIO Function Select: GPIO Pin X1 Takes Alternate Function 0
#define GPIO_FSEL1_ALT1                                       0x28 // GPIO Function Select: GPIO Pin X1 Takes Alternate Function 1
#define GPIO_FSEL1_ALT2                                       0x30 // GPIO Function Select: GPIO Pin X1 Takes Alternate Function 2
#define GPIO_FSEL1_ALT3                                       0x38 // GPIO Function Select: GPIO Pin X1 Takes Alternate Function 3
#define GPIO_FSEL1_ALT4                                       0x18 // GPIO Function Select: GPIO Pin X1 Takes Alternate Function 4
#define GPIO_FSEL1_ALT5                                       0x10 // GPIO Function Select: GPIO Pin X1 Takes Alternate Function 5
#define GPIO_FSEL1_CLR                                        0x38 // GPIO Function Select: GPIO Pin X1 Clear Bits
#define GPIO_FSEL2_IN                                           0x0 // GPIO Function Select: GPIO Pin X2 Is An Input
#define GPIO_FSEL2_OUT                                         0x40 // GPIO Function Select: GPIO Pin X2 Is An Output
#define GPIO_FSEL2_ALT0                                       0x100 // GPIO Function Select: GPIO Pin X2 Takes Alternate Function 0
#define GPIO_FSEL2_ALT1                                       0x140 // GPIO Function Select: GPIO Pin X2 Takes Alternate Function 1
#define GPIO_FSEL2_ALT2                                       0x180 // GPIO Function Select: GPIO Pin X2 Takes Alternate Function 2
#define GPIO_FSEL2_ALT3                                       0x1C0 // GPIO Function Select: GPIO Pin X2 Takes Alternate Function 3
#define GPIO_FSEL2_ALT4                                        0xC0 // GPIO Function Select: GPIO Pin X2 Takes Alternate Function 4
#define GPIO_FSEL2_ALT5                                        0x80 // GPIO Function Select: GPIO Pin X2 Takes Alternate Function 5
#define GPIO_FSEL2_CLR                                        0x1C0 // GPIO Function Select: GPIO Pin X2 Clear Bits
#define GPIO_FSEL3_IN                                           0x0 // GPIO Function Select: GPIO Pin X3 Is An Input
#define GPIO_FSEL3_OUT                                        0x200 // GPIO Function Select: GPIO Pin X3 Is An Output
#define GPIO_FSEL3_ALT0                                       0x800 // GPIO Function Select: GPIO Pin X3 Takes Alternate Function 0
#define GPIO_FSEL3_ALT1                                       0xA00 // GPIO Function Select: GPIO Pin X3 Takes Alternate Function 1
#define GPIO_FSEL3_ALT2                                       0xC00 // GPIO Function Select: GPIO Pin X3 Takes Alternate Function 2
#define GPIO_FSEL3_ALT3                                       0xE00 // GPIO Function Select: GPIO Pin X3 Takes Alternate Function 3
#define GPIO_FSEL3_ALT4                                       0x600 // GPIO Function Select: GPIO Pin X3 Takes Alternate Function 4
#define GPIO_FSEL3_ALT5                                       0x400 // GPIO Function Select: GPIO Pin X3 Takes Alternate Function 5
#define GPIO_FSEL3_CLR                                        0xE00 // GPIO Function Select: GPIO Pin X3 Clear Bits
#define GPIO_FSEL4_IN                                            0x0 // GPIO Function Select: GPIO Pin X4 Is An Input
#define GPIO_FSEL4_OUT                                        0x1000 // GPIO Function Select: GPIO Pin X4 Is An Output
#define GPIO_FSEL4_ALT0                                       0x4000 // GPIO Function Select: GPIO Pin X4 Takes Alternate Function 0
#define GPIO_FSEL4_ALT1                                       0x5000 // GPIO Function Select: GPIO Pin X4 Takes Alternate Function 1
#define GPIO_FSEL4_ALT2                                       0x6000 // GPIO Function Select: GPIO Pin X4 Takes Alternate Function 2
#define GPIO_FSEL4_ALT3                                       0x7000 // GPIO Function Select: GPIO Pin X4 Takes Alternate Function 3
#define GPIO_FSEL4_ALT4                                       0x3000 // GPIO Function Select: GPIO Pin X4 Takes Alternate Function 4
#define GPIO_FSEL4_ALT5                                       0x2000 // GPIO Function Select: GPIO Pin X4 Takes Alternate Function 5
#define GPIO_FSEL4_CLR                                        0x7000 // GPIO Function Select: GPIO Pin X4 Clear Bits
#define GPIO_FSEL5_IN                                             0x0 // GPIO Function Select: GPIO Pin X5 Is An Input
#define GPIO_FSEL5_OUT                                         0x8000 // GPIO Function Select: GPIO Pin X5 Is An Output
#define GPIO_FSEL5_ALT0                                       0x20000 // GPIO Function Select: GPIO Pin X5 Takes Alternate Function 0
#define GPIO_FSEL5_ALT1                                       0x28000 // GPIO Function Select: GPIO Pin X5 Takes Alternate Function 1
#define GPIO_FSEL5_ALT2                                       0x30000 // GPIO Function Select: GPIO Pin X5 Takes Alternate Function 2
#define GPIO_FSEL5_ALT3                                       0x38000 // GPIO Function Select: GPIO Pin X5 Takes Alternate Function 3
#define GPIO_FSEL5_ALT4                                       0x18000 // GPIO Function Select: GPIO Pin X5 Takes Alternate Function 4
#define GPIO_FSEL5_ALT5                                       0x10000 // GPIO Function Select: GPIO Pin X5 Takes Alternate Function 5
#define GPIO_FSEL5_CLR                                        0x38000 // GPIO Function Select: GPIO Pin X5 Clear Bits
#define GPIO_FSEL6_IN                                              0x0 // GPIO Function Select: GPIO Pin X6 Is An Input
#define GPIO_FSEL6_OUT                                         0x40000 // GPIO Function Select: GPIO Pin X6 Is An Output
#define GPIO_FSEL6_ALT0                                       0x100000 // GPIO Function Select: GPIO Pin X6 Takes Alternate Function 0
#define GPIO_FSEL6_ALT1                                       0x140000 // GPIO Function Select: GPIO Pin X6 Takes Alternate Function 1
#define GPIO_FSEL6_ALT2                                       0x180000 // GPIO Function Select: GPIO Pin X6 Takes Alternate Function 2
#define GPIO_FSEL6_ALT3                                       0x1C0000 // GPIO Function Select: GPIO Pin X6 Takes Alternate Function 3
#define GPIO_FSEL6_ALT4                                        0xC0000 // GPIO Function Select: GPIO Pin X6 Takes Alternate Function 4
#define GPIO_FSEL6_ALT5                                        0x80000 // GPIO Function Select: GPIO Pin X6 Takes Alternate Function 5
#define GPIO_FSEL6_CLR                                        0x1C0000 // GPIO Function Select: GPIO Pin X6 Clear Bits
#define GPIO_FSEL7_IN                                              0x0 // GPIO Function Select: GPIO Pin X7 Is An Input
#define GPIO_FSEL7_OUT                                        0x200000 // GPIO Function Select: GPIO Pin X7 Is An Output
#define GPIO_FSEL7_ALT0                                       0x800000 // GPIO Function Select: GPIO Pin X7 Takes Alternate Function 0
#define GPIO_FSEL7_ALT1                                       0xA00000 // GPIO Function Select: GPIO Pin X7 Takes Alternate Function 1
#define GPIO_FSEL7_ALT2                                       0xC00000 // GPIO Function Select: GPIO Pin X7 Takes Alternate Function 2
#define GPIO_FSEL7_ALT3                                       0xE00000 // GPIO Function Select: GPIO Pin X7 Takes Alternate Function 3
#define GPIO_FSEL7_ALT4                                       0x600000 // GPIO Function Select: GPIO Pin X7 Takes Alternate Function 4
#define GPIO_FSEL7_ALT5                                       0x400000 // GPIO Function Select: GPIO Pin X7 Takes Alternate Function 5
#define GPIO_FSEL7_CLR                                        0xE00000 // GPIO Function Select: GPIO Pin X7 Clear Bits
#define GPIO_FSEL8_IN                                               0x0 // GPIO Function Select: GPIO Pin X8 Is An Input
#define GPIO_FSEL8_OUT                                        0x1000000 // GPIO Function Select: GPIO Pin X8 Is An Output
#define GPIO_FSEL8_ALT0                                       0x4000000 // GPIO Function Select: GPIO Pin X8 Takes Alternate Function 0
#define GPIO_FSEL8_ALT1                                       0x5000000 // GPIO Function Select: GPIO Pin X8 Takes Alternate Function 1
#define GPIO_FSEL8_ALT2                                       0x6000000 // GPIO Function Select: GPIO Pin X8 Takes Alternate Function 2
#define GPIO_FSEL8_ALT3                                       0x7000000 // GPIO Function Select: GPIO Pin X8 Takes Alternate Function 3
#define GPIO_FSEL8_ALT4                                       0x3000000 // GPIO Function Select: GPIO Pin X8 Takes Alternate Function 4
#define GPIO_FSEL8_ALT5                                       0x2000000 // GPIO Function Select: GPIO Pin X8 Takes Alternate Function 5
#define GPIO_FSEL8_CLR                                        0x7000000 // GPIO Function Select: GPIO Pin X8 Clear Bits
#define GPIO_FSEL9_IN                                                0x0 // GPIO Function Select: GPIO Pin X9 Is An Input
#define GPIO_FSEL9_OUT                                         0x8000000 // GPIO Function Select: GPIO Pin X9 Is An Output
#define GPIO_FSEL9_ALT0                                       0x20000000 // GPIO Function Select: GPIO Pin X9 Takes Alternate Function 0
#define GPIO_FSEL9_ALT1                                       0x28000000 // GPIO Function Select: GPIO Pin X9 Takes Alternate Function 1
#define GPIO_FSEL9_ALT2                                       0x30000000 // GPIO Function Select: GPIO Pin X9 Takes Alternate Function 2
#define GPIO_FSEL9_ALT3                                       0x38000000 // GPIO Function Select: GPIO Pin X9 Takes Alternate Function 3
#define GPIO_FSEL9_ALT4                                       0x18000000 // GPIO Function Select: GPIO Pin X9 Takes Alternate Function 4
#define GPIO_FSEL9_ALT5                                       0x10000000 // GPIO Function Select: GPIO Pin X9 Takes Alternate Function 5
#define GPIO_FSEL9_CLR                                        0x38000000 // GPIO Function Select: GPIO Pin X9 Clear Bits


#define CM_BASE                                               (SUBSYSTEM_BASE + 0x101000) // Clock Manager Base Address
#define CM_GNRICCTL                                           ( CM_BASE + 0x000) // Clock Manager Generic Clock Control
#define CM_GNRICDIV                                           ( CM_BASE + 0x004) // Clock Manager Generic Clock Divisor
#define CM_VPUCTL                                             ( CM_BASE + 0x008) // Clock Manager VPU Clock Control
#define CM_VPUDIV                                             ( CM_BASE + 0x00C) // Clock Manager VPU Clock Divisor
#define CM_SYSCTL                                             ( CM_BASE + 0x010) // Clock Manager System Clock Control
#define CM_SYSDIV                                             ( CM_BASE + 0x014) // Clock Manager System Clock Divisor
#define CM_PERIACTL                                           ( CM_BASE + 0x018) // Clock Manager PERIA Clock Control
#define CM_PERIADIV                                           ( CM_BASE + 0x01C) // Clock Manager PERIA Clock Divisor
#define CM_PERIICTL                                           ( CM_BASE + 0x020) // Clock Manager PERII Clock Control
#define CM_PERIIDIV                                           ( CM_BASE + 0x024) // Clock Manager PERII Clock Divisor
#define CM_H264CTL                                            ( CM_BASE + 0x028) // Clock Manager H264 Clock Control
#define CM_H264DIV                                            ( CM_BASE + 0x02C) // Clock Manager H264 Clock Divisor
#define CM_ISPCTL                                             ( CM_BASE + 0x030) // Clock Manager ISP Clock Control
#define CM_ISPDIV                                             ( CM_BASE + 0x034) // Clock Manager ISP Clock Divisor
#define CM_V3DCTL                                             ( CM_BASE + 0x038) // Clock Manager V3D Clock Control
#define CM_V3DDIV                                             ( CM_BASE + 0x03C) // Clock Manager V3D Clock Divisor
#define CM_CAM0CTL                                            ( CM_BASE + 0x040) // Clock Manager Camera 0 Clock Control
#define CM_CAM0DIV                                            ( CM_BASE + 0x044) // Clock Manager Camera 0 Clock Divisor
#define CM_CAM1CTL                                            ( CM_BASE + 0x048) // Clock Manager Camera 1 Clock Control
#define CM_CAM1DIV                                            ( CM_BASE + 0x04C) // Clock Manager Camera 1 Clock Divisor
#define CM_CCP2CTL                                            ( CM_BASE + 0x050) // Clock Manager CCP2 Clock Control
#define CM_CCP2DIV                                            ( CM_BASE + 0x054) // Clock Manager CCP2 Clock Divisor
#define CM_DSI0ECTL                                           ( CM_BASE + 0x058) // Clock Manager DSI0E Clock Control
#define CM_DSI0EDIV                                           ( CM_BASE + 0x05C) // Clock Manager DSI0E Clock Divisor
#define CM_DSI0PCTL                                           ( CM_BASE + 0x060) // Clock Manager DSI0P Clock Control
#define CM_DSI0PDIV                                           ( CM_BASE + 0x064) // Clock Manager DSI0P Clock Divisor
#define CM_DPICTL                                             ( CM_BASE + 0x068) // Clock Manager DPI Clock Control
#define CM_DPIDIV                                             ( CM_BASE + 0x06C) // Clock Manager DPI Clock Divisor
#define CM_GP0CTL                                             ( CM_BASE + 0x070) // Clock Manager General Purpose 0 Clock Control
#define CM_GP0DIV                                             ( CM_BASE + 0x074) // Clock Manager General Purpose 0 Clock Divisor
#define CM_GP1CTL                                             ( CM_BASE + 0x078) // Clock Manager General Purpose 1 Clock Control
#define CM_GP1DIV                                             ( CM_BASE + 0x07C) // Clock Manager General Purpose 1 Clock Divisor
#define CM_GP2CTL                                             ( CM_BASE + 0x080) // Clock Manager General Purpose 2 Clock Control
#define CM_GP2DIV                                             ( CM_BASE + 0x084) // Clock Manager General Purpose 2 Clock Divisor
#define CM_HSMCTL                                             ( CM_BASE + 0x088) // Clock Manager HSM Clock Control
#define CM_HSMDIV                                             ( CM_BASE + 0x08C) // Clock Manager HSM Clock Divisor
#define CM_OTPCTL                                             ( CM_BASE + 0x090) // Clock Manager OTP Clock Control
#define CM_OTPDIV                                             ( CM_BASE + 0x094) // Clock Manager OTP Clock Divisor
#define CM_PCMCTL                                             ( CM_BASE + 0x098) // Clock Manager PCM / I2S Clock Control
#define CM_PCMDIV                                             ( CM_BASE + 0x09C) // Clock Manager PCM / I2S Clock Divisor
#define CM_PWMCTL                                             ( CM_BASE + 0x0A0) // Clock Manager PWM Clock Control
#define CM_PWMDIV                                             ( CM_BASE + 0x0A4) // Clock Manager PWM Clock Divisor
#define CM_SLIMCTL                                            ( CM_BASE + 0x0A8) // Clock Manager SLIM Clock Control
#define CM_SLIMDIV                                            ( CM_BASE + 0x0AC) // Clock Manager SLIM Clock Divisor
#define CM_SMICTL                                             ( CM_BASE + 0x0B0) // Clock Manager SMI Clock Control
#define CM_SMIDIV                                             ( CM_BASE + 0x0B4) // Clock Manager SMI Clock Divisor
#define CM_TCNTCTL                                            ( CM_BASE + 0x0C0) // Clock Manager TCNT Clock Control
#define CM_TCNTDIV                                            ( CM_BASE + 0x0C4) // Clock Manager TCNT Clock Divisor
#define CM_TECCTL                                             ( CM_BASE + 0x0C8) // Clock Manager TEC Clock Control
#define CM_TECDIV                                             ( CM_BASE + 0x0CC) // Clock Manager TEC Clock Divisor
#define CM_TD0CTL                                             ( CM_BASE + 0x0D0) // Clock Manager TD0 Clock Control
#define CM_TD0DIV                                             ( CM_BASE + 0x0D4) // Clock Manager TD0 Clock Divisor
#define CM_TD1CTL                                             ( CM_BASE + 0x0D8) // Clock Manager TD1 Clock Control
#define CM_TD1DIV                                             ( CM_BASE + 0x0DC) // Clock Manager TD1 Clock Divisor
#define CM_TSENSCTL                                           ( CM_BASE + 0x0E0) // Clock Manager TSENS Clock Control
#define CM_TSENSDIV                                           ( CM_BASE + 0x0E4) // Clock Manager TSENS Clock Divisor
#define CM_TIMERCTL                                           ( CM_BASE + 0x0E8) // Clock Manager Timer Clock Control
#define CM_TIMERDIV                                           ( CM_BASE + 0x0EC) // Clock Manager Timer Clock Divisor
#define CM_UARTCTL                                            ( CM_BASE + 0x0F0) // Clock Manager UART Clock Control
#define CM_UARTDIV                                            ( CM_BASE + 0x0F4) // Clock Manager UART Clock Divisor
#define CM_VECCTL                                             ( CM_BASE + 0x0F8) // Clock Manager VEC Clock Control
#define CM_VECDIV                                             ( CM_BASE + 0x0FC) // Clock Manager VEC Clock Divisor
#define CM_OSCCOUNT                                           ( CM_BASE + 0x100) // Clock Manager Oscillator Count
#define CM_PLLA                                               ( CM_BASE + 0x104) // Clock Manager PLLA
#define CM_PLLC                                               ( CM_BASE + 0x108) // Clock Manager PLLC
#define CM_PLLD                                               ( CM_BASE + 0x10C) // Clock Manager PLLD
#define CM_PLLH                                               ( CM_BASE + 0x110) // Clock Manager PLLH
#define CM_LOCK                                               ( CM_BASE + 0x114) // Clock Manager Lock
#define CM_EVENT                                              ( CM_BASE + 0x118) // Clock Manager Event
#define CM_INTEN                                              ( CM_BASE + 0x118) // Clock Manager INTEN
#define CM_DSI0HSCK                                           ( CM_BASE + 0x120) // Clock Manager DSI0HSCK
#define CM_CKSM                                               ( CM_BASE + 0x124) // Clock Manager CKSM
#define CM_OSCFREQI                                           ( CM_BASE + 0x128) // Clock Manager Oscillator Frequency Integer
#define CM_OSCFREQF                                           ( CM_BASE + 0x12C) // Clock Manager Oscillator Frequency Fraction
#define CM_PLLTCTL                                            ( CM_BASE + 0x130) // Clock Manager PLLT Control
#define CM_PLLTCNT0                                           ( CM_BASE + 0x134) // Clock Manager PLLT0
#define CM_PLLTCNT1                                           ( CM_BASE + 0x138) // Clock Manager PLLT1
#define CM_PLLTCNT2                                           ( CM_BASE + 0x13C) // Clock Manager PLLT2
#define CM_PLLTCNT3                                           ( CM_BASE + 0x140) // Clock Manager PLLT3
#define CM_TDCLKEN                                            ( CM_BASE + 0x144) // Clock Manager TD Clock Enable
#define CM_BURSTCTL                                           ( CM_BASE + 0x148) // Clock Manager Burst Control
#define CM_BURSTCNT                                           ( CM_BASE + 0x14C) // Clock Manager Burst
#define CM_DSI1ECTL                                           ( CM_BASE + 0x158) // Clock Manager DSI1E Clock Control
#define CM_DSI1EDIV                                           ( CM_BASE + 0x15C) // Clock Manager DSI1E Clock Divisor
#define CM_DSI1PCTL                                           ( CM_BASE + 0x160) // Clock Manager DSI1P Clock Control
#define CM_DSI1PDIV                                           ( CM_BASE + 0x164) // Clock Manager DSI1P Clock Divisor
#define CM_DFTCTL                                             ( CM_BASE + 0x168) // Clock Manager DFT Clock Control
#define CM_DFTDIV                                             ( CM_BASE + 0x16C) // Clock Manager DFT Clock Divisor
#define CM_PLLB                                               ( CM_BASE + 0x170) // Clock Manager PLLB
#define CM_PULSECTL                                           ( CM_BASE + 0x190) // Clock Manager Pulse Clock Control
#define CM_PULSEDIV                                           ( CM_BASE + 0x194) // Clock Manager Pulse Clock Divisor
#define CM_SDCCTL                                             ( CM_BASE + 0x1A8) // Clock Manager SDC Clock Control
#define CM_SDCDIV                                             ( CM_BASE + 0x1AC) // Clock Manager SDC Clock Divisor
#define CM_ARMCTL                                             ( CM_BASE + 0x1B0) // Clock Manager ARM Clock Control
#define CM_ARMDIV                                             ( CM_BASE + 0x1B4) // Clock Manager ARM Clock Divisor
#define CM_AVEOCTL                                            ( CM_BASE + 0x1B8) // Clock Manager AVEO Clock Control
#define CM_AVEODIV                                            ( CM_BASE + 0x1BC) // Clock Manager AVEO Clock Divisor
#define CM_EMMCCTL                                            ( CM_BASE + 0x1C0) // Clock Manager EMMC Clock Control
#define CM_EMMCDIV                                            ( CM_BASE + 0x1C4) // Clock Manager EMMC Clock Divisor
#define CM_SRC_OSCILLATOR                                     (            0x01) // Clock Control: Clock Source      Oscillator
#define CM_SRC_TESTDEBUG0                                     (            0x02) // Clock Control: Clock Source      Test Debug 0
#define CM_SRC_TESTDEBUG1                                     (            0x03) // Clock Control: Clock Source      Test Debug 1
#define CM_SRC_PLLAPER                                        (            0x04) // Clock Control: Clock Source      PLLA Per
#define CM_SRC_PLLCPER                                        (            0x05) // Clock Control: Clock Source      PLLC Per
#define CM_SRC_PLLDPER                                        (            0x06) // Clock Control: Clock Source      PLLD Per
#define CM_SRC_HDMIAUX                                        (            0x07) // Clock Control: Clock Source      HDMI Auxiliary
#define CM_SRC_GND                                            (            0x08) // Clock Control: Clock Source      GND
#define CM_ENAB                                               (            0x10) // Clock Control: Enable The Clock Generator
#define CM_KILL                                               (            0x20) // Clock Control: Kill The Clock Generator
#define CM_BUSY                                               (            0x80) // Clock Control: Clock Generator Is Running
#define CM_FLIP                                               (           0x100) // Clock Control: Invert The Clock Generator Output
#define CM_MASH_1                                             (           0x200) // Clock Control: MASH Control      1-Stage MASH (Equivalent To Non-MASH Dividers)
#define CM_MASH_2                                             (           0x400) // Clock Control: MASH Control      2-Stage MASH
#define CM_MASH_3                                             (           0x600) // Clock Control: MASH Control      3-Stage MASH
#define CM_PASSWORD                                           (      0x5A000000) // Clock Control: Password "5A"


//#define GPIO_STATUS_LED                                       16 //PI1
#define GPIO_STATUS_LED                                       (47 % 32) //PI2

#define SYST_BASE                                             (SUBSYSTEM_BASE + 0x3000)
#define SYST_CLO                                              (SYST_BASE + 0x04)
#define SYST_CHI                                              (SYST_BASE + 0x08)

#define UART0_BASE                                            (SUBSYSTEM_BASE + 0x00201000)
#define UART0_DR                                              (UART0_BASE + 0x00)
#define UART0_RSRECR                                          (UART0_BASE + 0x04)
#define UART0_FR                                              (UART0_BASE + 0x18)
#define UART0_ILPR                                            (UART0_BASE + 0x20)
#define UART0_IBRD                                            (UART0_BASE + 0x24)
#define UART0_FBRD                                            (UART0_BASE + 0x28)
#define UART0_LCRH                                            (UART0_BASE + 0x2C)
#define UART0_CR                                              (UART0_BASE + 0x30)
#define UART0_IFLS                                            (UART0_BASE + 0x34)
#define UART0_IMSC                                            (UART0_BASE + 0x38)
#define UART0_RIS                                             (UART0_BASE + 0x3C)
#define UART0_MIS                                             (UART0_BASE + 0x40)
#define UART0_ICR                                             (UART0_BASE + 0x44)
#define UART0_DMACR                                           (UART0_BASE + 0x48)
#define UART0_ITCR                                            (UART0_BASE + 0x80)
#define UART0_ITIP                                            (UART0_BASE + 0x84)
#define UART0_ITOP                                            (UART0_BASE + 0x88)
#define UART0_TDR                                             (UART0_BASE + 0x8C)

#define MAILBOX_BASE                                          (SUBSYSTEM_BASE + 0xB880)
#define MAILBOX_READ                                          (MAILBOX_BASE   + 0x00)
#define MAILBOX_STATUS                                        (MAILBOX_BASE   + 0x18)
#define MAILBOX_CONFIG                                        (MAILBOX_BASE   + 0x1C)
#define MAILBOX_WRITE                                         (MAILBOX_BASE   + 0x20)

#define MAILBOX_FULL                                          0x80000000
#define MAILBOX_EMPTY                                         0x40000000
#define MAILBOX_FRAMEBUFFER                                   1
#define MAILBOX_VCHIQ                                         3
#define MAILBOX_ARM_TO_VC                                     8
#define MAILBOX_VC_TO_ARM                                     9

//#define VCADDR_BASE                                           0
//#define VCADDR_BASE                                           0x40000000
#define VCADDR_BASE                                            0xC0000000
#define ENABLE_MSAAx4                                          1
#define TILE_CTRLLIST_HALT                                     0
#define TILE_CTRLLIST_NOP                                      1
#define TILE_CTRLLIST_FLUSH                                    4
#define TILE_CTRLLIST_FLUSH_ALL                                5
#define TILE_CTRLLIST_START_BINNING                            6
#define TILE_CTRLLIST_BRANCH_TO_SUBLIST                        17
#define TILE_CTRLLIST_STORE_MS_RESOLVED_BUFFER                 24
#define TILE_CTRLLIST_STORE_MS_RESOLVED_BUFFER_AND_END_FRAME   25
#define TILE_CTRLLIST_STORE_GENERAL                            28
#define TILE_CTRLLIST_PRIM_INDEX_LIST                          32
#define TILE_CTRLLIST_VERTEX_ARRAY_PRIM                        33
#define TILE_CTRLLIST_PRIM_TYPE                                56
#define TILE_CTRLLIST_NV_SHADER_PRE                            65
#define TILE_CTRLLIST_NV_SHADER                                66
#define TILE_CTRLLIST_STATE                                    96
#define TILE_CTRLLIST_CLIP_WINDOW                              102
#define TILE_CTRLLIST_VIEWPORT_OFFSET                          103
#define TILE_CTRLLIST_BINNING_CONFIG                           112
#define TILE_CTRLLIST_RENDER_MODE_CONFIG                       113
#define TILE_CTRLLIST_CLEAR_COLOR                              114
#define TILE_CTRLLIST_RENDER_TILE_COORD                        115
#define TILE_SIZE                                              32

//BINNING CONFIG
#define TILE_CTRLLIST_BINNING_MS                               0x01
#define TILE_CTRLLIST_BINNING_AUTO                             0x04
#define TILE_CTRLLIST_BINNING_DEFAULT                          (TILE_CTRLLIST_BINNING_AUTO)

//RENDERING CONFIG
#define TILE_CTRLLIST_RENDER_CFG_MS                            0x01
#define TILE_CTRLLIST_RENDER_CFG_RGBA8888                      0x04
#define TILE_CTRLLIST_RENDER_DEFAULT                           (TILE_CTRLLIST_RENDER_CFG_RGBA8888)


#define TILE_COLOR_BITS                                        32
#define TILE_UNIT_BYTES                                        (TILE_SIZE * TILE_SIZE * 4)
#define TILE_MEM_ALLOC_ALIGN                                   (0x1000)
#define TILE_MEM_ALLOC_OPTION                                  (MAILBOX_MEM_FLAG_COHERENT | MAILBOX_MEM_FLAG_ZERO)

//MEMORYMAP
#define TILE_MEM_OFFSET_BINNIG_CTRL_LIST                       (0x00000000)
#define TILE_MEM_OFFSET_BINNIG_CTRL_LIST_END                   (0x00007FFF)
#define TILE_MEM_OFFSET_RENDER_CTRL_LIST                       (0x00008000)
#define TILE_MEM_OFFSET_RENDER_CTRL_LIST_END                   (0x0000FFFF)
#define TILE_MEM_OFFSET_SHADER_INFO                            (0x00010000)
#define TILE_MEM_OFFSET_STATE                                  (0x00020000)
#define TILE_MEM_OFFSET_VSHADER                                (0x00080000)
#define TILE_MEM_OFFSET_FSHADER                                (0x00090000)
#define TILE_MEM_OFFSET_VSHADER_UNIFORM                        (0x000A0000)
#define TILE_MEM_OFFSET_FSHADER_UNIFORM                        (0x000A8000)
#define TILE_MEM_OFFSET_INDEX_BUFFER                           (0x000B0000)
#define TILE_MEM_OFFSET_VERTEX_BUFFER                          (0x001B0000)
#define TILE_MEM_OFFSET_BINNING_DATA                           (0x00200000)
#define TILE_MEM_OFFSET_MAX                                    (0x00800000)
#define TILE_MEM_ALLOC_SIZE                                    (TILE_MEM_OFFSET_MAX)
#define TILE_MEM_ALLOC_PAGE_NUM                                2

#define TILE_CTRLLIST_STATE_ENABLE_FORWARD_FACING_PRIM         (1 << 0)
#define TILE_CTRLLIST_STATE_ENABLE_REVERSE_FACING_PRIM         (1 << 1)
#define TILE_CTRLLIST_STATE_CLOCKWISE_PRIM                     (1 << 2)
#define TILE_CTRLLIST_STATE_ENABLE_DEPTH_OFFSET                (1 << 3)
#define TILE_CTRLLIST_STATE_AA_POINT_LINE                      (1 << 4) //not sup
#define TILE_CTRLLIST_STATE_COVERAGE_READ_TYPE                 (1 << 5)
#define TILE_CTRLLIST_STATE_RASTERISER_OVERSAMPLE_MODE_4x      (1 << 6)
#define TILE_CTRLLIST_STATE_RASTERISER_OVERSAMPLE_MODE_16x     (1 << 7)


//https://github.com/PeterLemon/RaspberryPi/blob/master/HelloWorld/DMA/LIB/R_PI2.INC
// DMA Controller
#define DMA0_BASE                                              (SUBSYSTEM_BASE + 0x00007000) // DMA Channel 0 Register Set
#define DMA1_BASE                                              (SUBSYSTEM_BASE + 0x00007100) // DMA Channel 1 Register Set
#define DMA2_BASE                                              (SUBSYSTEM_BASE + 0x00007200) // DMA Channel 2 Register Set
#define DMA3_BASE                                              (SUBSYSTEM_BASE + 0x00007300) // DMA Channel 3 Register Set
#define DMA4_BASE                                              (SUBSYSTEM_BASE + 0x00007400) // DMA Channel 4 Register Set
#define DMA5_BASE                                              (SUBSYSTEM_BASE + 0x00007500) // DMA Channel 5 Register Set
#define DMA6_BASE                                              (SUBSYSTEM_BASE + 0x00007600) // DMA Channel 6 Register Set
#define DMA7_BASE                                              (SUBSYSTEM_BASE + 0x00007700) // DMA Channel 7 Register Set
#define DMA8_BASE                                              (SUBSYSTEM_BASE + 0x00007800) // DMA Channel 8 Register Set
#define DMA9_BASE                                              (SUBSYSTEM_BASE + 0x00007900) // DMA Channel 9 Register Set
#define DMA10_BASE                                             (SUBSYSTEM_BASE + 0x00007A00) // DMA Channel 10 Register Set
#define DMA11_BASE                                             (SUBSYSTEM_BASE + 0x00007B00) // DMA Channel 11 Register Set
#define DMA12_BASE                                             (SUBSYSTEM_BASE + 0x00007C00) // DMA Channel 12 Register Set
#define DMA13_BASE                                             (SUBSYSTEM_BASE + 0x00007D00) // DMA Channel 13 Register Set
#define DMA14_BASE                                             (SUBSYSTEM_BASE + 0x00007E00) // DMA Channel 14 Register Set
#define DMA_INT_STATUS                                         0x00007FE0 // Interrupt Status of each DMA Channel
#define DMA_ENABLE                                             0x00007FF0 // Global Enable bits for each DMA Channel
#define DMA15_BASE                                             0x00E05000 // DMA Channel 15 Register Set

#define DMA_CS                                                 0x00000000 // DMA Channel 0..14 Control & Status
#define DMA_CONBLK_AD                                          0x00000004 // DMA Channel 0..14 Control Block Address
#define DMA_TI                                                 0x00000008 // DMA Channel 0..14 CB Word 0 (Transfer Information)
#define DMA_SOURCE_AD                                          0x0000000C // DMA Channel 0..14 CB Word 1 (Source Address)
#define DMA_DEST_AD                                            0x00000010 // DMA Channel 0..14 CB Word 2 (Destination Address)
#define DMA_TXFR_LEN                                           0x00000014 // DMA Channel 0..14 CB Word 3 (Transfer Length)
#define DMA_STRIDE                                             0x00000018 // DMA Channel 0..14 CB Word 4 (2D Stride)
#define DMA_NEXTCONBK                                          0x0000001C // DMA Channel 0..14 CB Word 5 (Next CB Address)
#define DMA_DEBUG                                              0x00000020 // DMA Channel 0..14 Debug

#define DMA_ACTIVE                                             0x00000001 // DMA Control & Status: Activate the DMA
#define DMA_END                                                0x00000002 // DMA Control & Status: DMA End Flag
#define DMA_INT                                                0x00000004 // DMA Control & Status: Interrupt Status
#define DMA_DREQ                                               0x00000008 // DMA Control & Status: DREQ State
#define DMA_PAUSED                                             0x00000010 // DMA Control & Status: DMA Paused State
#define DMA_DREQ_STOPS_DMA                                     0x00000020 // DMA Control & Status: DMA Paused by DREQ State
#define DMA_WAITING_FOR_OUTSTANDING_WRITES                     0x00000040 // DMA Control & Status: DMA is Waiting for the Last Write to be Received

#define DMA_ERROR                                              0x00000100 // DMA Control & Status: DMA Error
#define DMA_PRIORITY_0                                         0x00000000 // DMA Control & Status: AXI Priority Level 0
#define DMA_PRIORITY_1                                         0x00010000 // DMA Control & Status: AXI Priority Level 1
#define DMA_PRIORITY_2                                         0x00020000 // DMA Control & Status: AXI Priority Level 2
#define DMA_PRIORITY_3                                         0x00030000 // DMA Control & Status: AXI Priority Level 3
#define DMA_PRIORITY_4                                         0x00040000 // DMA Control & Status: AXI Priority Level 4
#define DMA_PRIORITY_5                                         0x00050000 // DMA Control & Status: AXI Priority Level 5
#define DMA_PRIORITY_6                                         0x00060000 // DMA Control & Status: AXI Priority Level 6
#define DMA_PRIORITY_7                                         0x00070000 // DMA Control & Status: AXI Priority Level 7
#define DMA_PRIORITY_8                                         0x00080000 // DMA Control & Status: AXI Priority Level 8
#define DMA_PRIORITY_9                                         0x00090000 // DMA Control & Status: AXI Priority Level 9
#define DMA_PRIORITY_10                                        0x000A0000 // DMA Control & Status: AXI Priority Level 10
#define DMA_PRIORITY_11                                        0x000B0000 // DMA Control & Status: AXI Priority Level 11
#define DMA_PRIORITY_12                                        0x000C0000 // DMA Control & Status: AXI Priority Level 12
#define DMA_PRIORITY_13                                        0x000D0000 // DMA Control & Status: AXI Priority Level 13
#define DMA_PRIORITY_14                                        0x000E0000 // DMA Control & Status: AXI Priority Level 14
#define DMA_PRIORITY_15                                        0x000F0000 // DMA Control & Status: AXI Priority Level 15
#define DMA_PRIORITY                                           0x000F0000 // DMA Control & Status: AXI Priority Level
#define DMA_PANIC_PRIORITY_0                                   0x00000000 // DMA Control & Status: AXI Panic Priority Level 0
#define DMA_PANIC_PRIORITY_1                                   0x00100000 // DMA Control & Status: AXI Panic Priority Level 1
#define DMA_PANIC_PRIORITY_2                                   0x00200000 // DMA Control & Status: AXI Panic Priority Level 2
#define DMA_PANIC_PRIORITY_3                                   0x00300000 // DMA Control & Status: AXI Panic Priority Level 3
#define DMA_PANIC_PRIORITY_4                                   0x00400000 // DMA Control & Status: AXI Panic Priority Level 4
#define DMA_PANIC_PRIORITY_5                                   0x00500000 // DMA Control & Status: AXI Panic Priority Level 5
#define DMA_PANIC_PRIORITY_6                                   0x00600000 // DMA Control & Status: AXI Panic Priority Level 6
#define DMA_PANIC_PRIORITY_7                                   0x00700000 // DMA Control & Status: AXI Panic Priority Level 7
#define DMA_PANIC_PRIORITY_8                                   0x00800000 // DMA Control & Status: AXI Panic Priority Level 8
#define DMA_PANIC_PRIORITY_9                                   0x00900000 // DMA Control & Status: AXI Panic Priority Level 9
#define DMA_PANIC_PRIORITY_10                                  0x00A00000 // DMA Control & Status: AXI Panic Priority Level 10
#define DMA_PANIC_PRIORITY_11                                  0x00B00000 // DMA Control & Status: AXI Panic Priority Level 11
#define DMA_PANIC_PRIORITY_12                                  0x00C00000 // DMA Control & Status: AXI Panic Priority Level 12
#define DMA_PANIC_PRIORITY_13                                  0x00D00000 // DMA Control & Status: AXI Panic Priority Level 13
#define DMA_PANIC_PRIORITY_14                                  0x00E00000 // DMA Control & Status: AXI Panic Priority Level 14
#define DMA_PANIC_PRIORITY_15                                  0x00F00000 // DMA Control & Status: AXI Panic Priority Level 14
#define DMA_PANIC_PRIORITY                                     0x00F00000 // DMA Control & Status: AXI Panic Priority Level
#define DMA_WAIT_FOR_OUTSTANDING_WRITES                        0x10000000 // DMA Control & Status: Wait for Outstanding Writes
#define DMA_DISDEBUG                                           0x20000000 // DMA Control & Status: Disable Debug Pause Signal
#define DMA_ABORT                                              0x40000000 // DMA Control & Status: Abort DMA
#define DMA_RESET                                              0x80000000 // DMA Control & Status: DMA Channel Reset
#define DMA_INTEN                                              0x00000001 // DMA Transfer Information: Interrupt Enable
#define DMA_TDMODE                                             0x00000002 // DMA Transfer Information: 2D Mode
#define DMA_WAIT_RESP                                          0x00000008 // DMA Transfer Information: Wait for a Write Response
#define DMA_DEST_INC                                           0x00000010 // DMA Transfer Information: Destination Address Increment
#define DMA_DEST_WIDTH                                         0x00000020 // DMA Transfer Information: Destination Transfer Width
#define DMA_DEST_DREQ                                          0x00000040 // DMA Transfer Information: Control Destination Writes with DREQ
#define DMA_DEST_IGNORE                                        0x00000080 // DMA Transfer Information: Ignore Writes
#define DMA_SRC_INC                                            0x00000100 // DMA Transfer Information: Source Address Increment
#define DMA_SRC_WIDTH                                          0x00000200 // DMA Transfer Information: Source Transfer Width
#define DMA_SRC_DREQ                                           0x00000400 // DMA Transfer Information: Control Source Reads with DREQ
#define DMA_SRC_IGNORE                                         0x00000800 // DMA Transfer Information: Ignore Reads
#define DMA_BURST_LENGTH_1                                     0x00000000 // DMA Transfer Information: Burst Transfer Length 1 Word
#define DMA_BURST_LENGTH_2                                     0x00001000 // DMA Transfer Information: Burst Transfer Length 2 Words
#define DMA_BURST_LENGTH_3                                     0x00002000 // DMA Transfer Information: Burst Transfer Length 3 Words
#define DMA_BURST_LENGTH_4                                     0x00003000 // DMA Transfer Information: Burst Transfer Length 4 Words
#define DMA_BURST_LENGTH_5                                     0x00004000 // DMA Transfer Information: Burst Transfer Length 5 Words
#define DMA_BURST_LENGTH_6                                     0x00005000 // DMA Transfer Information: Burst Transfer Length 6 Words
#define DMA_BURST_LENGTH_7                                     0x00006000 // DMA Transfer Information: Burst Transfer Length 7 Words
#define DMA_BURST_LENGTH_8                                     0x00007000 // DMA Transfer Information: Burst Transfer Length 8 Words
#define DMA_BURST_LENGTH_9                                     0x00008000 // DMA Transfer Information: Burst Transfer Length 9 Words
#define DMA_BURST_LENGTH_10                                    0x00009000 // DMA Transfer Information: Burst Transfer Length 10 Words
#define DMA_BURST_LENGTH_11                                    0x0000A000 // DMA Transfer Information: Burst Transfer Length 11 Words
#define DMA_BURST_LENGTH_12                                    0x0000B000 // DMA Transfer Information: Burst Transfer Length 12 Words
#define DMA_BURST_LENGTH_13                                    0x0000C000 // DMA Transfer Information: Burst Transfer Length 13 Words
#define DMA_BURST_LENGTH_14                                    0x0000D000 // DMA Transfer Information: Burst Transfer Length 14 Words
#define DMA_BURST_LENGTH_15                                    0x0000E000 // DMA Transfer Information: Burst Transfer Length 15 Words
#define DMA_BURST_LENGTH_16                                    0x0000F000 // DMA Transfer Information: Burst Transfer Length 16 Words
#define DMA_BURST_LENGTH                                       0x0000F000 // DMA Transfer Information: Burst Transfer Length
#define DMA_PERMAP_0                                           0x00000000 // DMA Transfer Information: Peripheral Mapping Continuous Un-paced Transfer
#define DMA_PERMAP_1                                           0x00010000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 1
#define DMA_PERMAP_2                                           0x00020000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 2
#define DMA_PERMAP_3                                           0x00030000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 3
#define DMA_PERMAP_4                                           0x00040000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 4
#define DMA_PERMAP_5                                           0x00050000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 5
#define DMA_PERMAP_6                                           0x00060000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 6
#define DMA_PERMAP_7                                           0x00070000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 7
#define DMA_PERMAP_8                                           0x00080000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 8
#define DMA_PERMAP_9                                           0x00090000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 9
#define DMA_PERMAP_10                                          0x000A0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 10
#define DMA_PERMAP_11                                          0x000B0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 11
#define DMA_PERMAP_12                                          0x000C0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 12
#define DMA_PERMAP_13                                          0x000D0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 13
#define DMA_PERMAP_14                                          0x000E0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 14
#define DMA_PERMAP_15                                          0x000F0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 15
#define DMA_PERMAP_16                                          0x00100000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 16
#define DMA_PERMAP_17                                          0x00110000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 17
#define DMA_PERMAP_18                                          0x00120000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 18
#define DMA_PERMAP_19                                          0x00130000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 19
#define DMA_PERMAP_20                                          0x00140000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 20
#define DMA_PERMAP_21                                          0x00150000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 21
#define DMA_PERMAP_22                                          0x00160000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 22
#define DMA_PERMAP_23                                          0x00170000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 23
#define DMA_PERMAP_24                                          0x00180000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 24
#define DMA_PERMAP_25                                          0x00190000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 25
#define DMA_PERMAP_26                                          0x001A0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 26
#define DMA_PERMAP_27                                          0x001B0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 27
#define DMA_PERMAP_28                                          0x001C0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 28
#define DMA_PERMAP_29                                          0x001D0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 29
#define DMA_PERMAP_30                                          0x001E0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 30
#define DMA_PERMAP_31                                          0x001F0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 31
#define DMA_PERMAP                                             0x001F0000 // DMA Transfer Information: Peripheral Mapping
#define DMA_WAITS_0                                            0x00000000 // DMA Transfer Information: Add No Wait Cycles
#define DMA_WAITS_1                                            0x00200000 // DMA Transfer Information: Add 1 Wait Cycle
#define DMA_WAITS_2                                            0x00400000 // DMA Transfer Information: Add 2 Wait Cycles
#define DMA_WAITS_3                                            0x00600000 // DMA Transfer Information: Add 3 Wait Cycles
#define DMA_WAITS_4                                            0x00800000 // DMA Transfer Information: Add 4 Wait Cycles
#define DMA_WAITS_5                                            0x00A00000 // DMA Transfer Information: Add 5 Wait Cycles
#define DMA_WAITS_6                                            0x00C00000 // DMA Transfer Information: Add 6 Wait Cycles
#define DMA_WAITS_7                                            0x00E00000 // DMA Transfer Information: Add 7 Wait Cycles
#define DMA_WAITS_8                                            0x01000000 // DMA Transfer Information: Add 8 Wait Cycles
#define DMA_WAITS_9                                            0x01200000 // DMA Transfer Information: Add 9 Wait Cycles
#define DMA_WAITS_10                                           0x01400000 // DMA Transfer Information: Add 10 Wait Cycles
#define DMA_WAITS_11                                           0x01600000 // DMA Transfer Information: Add 11 Wait Cycles
#define DMA_WAITS_12                                           0x01800000 // DMA Transfer Information: Add 12 Wait Cycles
#define DMA_WAITS_13                                           0x01A00000 // DMA Transfer Information: Add 13 Wait Cycles
#define DMA_WAITS_14                                           0x01C00000 // DMA Transfer Information: Add 14 Wait Cycles
#define DMA_WAITS_15                                           0x01E00000 // DMA Transfer Information: Add 15 Wait Cycles
#define DMA_WAITS_16                                           0x02000000 // DMA Transfer Information: Add 16 Wait Cycles
#define DMA_WAITS_17                                           0x02200000 // DMA Transfer Information: Add 17 Wait Cycles
#define DMA_WAITS_18                                           0x02400000 // DMA Transfer Information: Add 18 Wait Cycles
#define DMA_WAITS_19                                           0x02600000 // DMA Transfer Information: Add 19 Wait Cycles
#define DMA_WAITS_20                                           0x02800000 // DMA Transfer Information: Add 20 Wait Cycles
#define DMA_WAITS_21                                           0x02A00000 // DMA Transfer Information: Add 21 Wait Cycles
#define DMA_WAITS_22                                           0x02C00000 // DMA Transfer Information: Add 22 Wait Cycles
#define DMA_WAITS_23                                           0x02E00000 // DMA Transfer Information: Add 23 Wait Cycles
#define DMA_WAITS_24                                           0x03000000 // DMA Transfer Information: Add 24 Wait Cycles
#define DMA_WAITS_25                                           0x03200000 // DMA Transfer Information: Add 25 Wait Cycles
#define DMA_WAITS_26                                           0x03400000 // DMA Transfer Information: Add 26 Wait Cycles
#define DMA_WAITS_27                                           0x03600000 // DMA Transfer Information: Add 27 Wait Cycles
#define DMA_WAITS_28                                           0x03800000 // DMA Transfer Information: Add 28 Wait Cycles
#define DMA_WAITS_29                                           0x03A00000 // DMA Transfer Information: Add 29 Wait Cycles
#define DMA_WAITS_30                                           0x03C00000 // DMA Transfer Information: Add 30 Wait Cycles
#define DMA_WAITS_31                                           0x03E00000 // DMA Transfer Information: Add 31 Wait Cycles
#define DMA_WAITS                                              0x03E00000 // DMA Transfer Information: Add Wait Cycles
#define DMA_NO_WIDE_BURSTS                                     0x04000000 // DMA Transfer Information: Don't Do Wide Writes as a 2 Beat Burst
#define DMA_XLENGTH                                            0x0000FFFF // DMA Transfer Length: Transfer Length in Bytes
#define DMA_YLENGTH                                            0x3FFF0000 // DMA Transfer Length: When in 2D Mode, This is the Y Transfer Length
#define DMA_S_STRIDE                                           0x0000FFFF // DMA 2D Stride: Source Stride (2D Mode)
#define DMA_D_STRIDE                                           0xFFFF0000 // DMA 2D Stride: Destination Stride (2D Mode)
#define DMA_READ_LAST_NOT_SET_ERROR                            0x00000001 // DMA Debug: Read Last Not Set Error
#define DMA_FIFO_ERROR                                         0x00000002 // DMA Debug: Fifo Error
#define DMA_READ_ERROR                                         0x00000004 // DMA Debug: Slave Read Response Error
#define DMA_OUTSTANDING_WRITES                                 0x000000F0 // DMA Debug: DMA Outstanding Writes Counter
#define DMA_ID                                                 0x0000FF00 // DMA Debug: DMA ID
#define DMA_STATE                                              0x01FF0000 // DMA Debug: DMA State Machine State
#define DMA_VERSION                                            0x0E000000 // DMA Debug: DMA Version
#define DMA_LITE                                               0x10000000 // DMA Debug: DMA Lite
#define DMA_INT0                                               0x00000001 // DMA Interrupt Status: Interrupt Status of DMA Engine 0
#define DMA_INT1                                               0x00000002 // DMA Interrupt Status: Interrupt Status of DMA Engine 1
#define DMA_INT2                                               0x00000004 // DMA Interrupt Status: Interrupt Status of DMA Engine 2
#define DMA_INT3                                               0x00000008 // DMA Interrupt Status: Interrupt Status of DMA Engine 3
#define DMA_INT4                                               0x00000010 // DMA Interrupt Status: Interrupt Status of DMA Engine 4
#define DMA_INT5                                               0x00000020 // DMA Interrupt Status: Interrupt Status of DMA Engine 5
#define DMA_INT6                                               0x00000040 // DMA Interrupt Status: Interrupt Status of DMA Engine 6
#define DMA_INT7                                               0x00000080 // DMA Interrupt Status: Interrupt Status of DMA Engine 7
#define DMA_INT8                                               0x00000100 // DMA Interrupt Status: Interrupt Status of DMA Engine 8
#define DMA_INT9                                               0x00000200 // DMA Interrupt Status: Interrupt Status of DMA Engine 9
#define DMA_INT10                                              0x00000400 // DMA Interrupt Status: Interrupt Status of DMA Engine 10
#define DMA_INT11                                              0x00000800 // DMA Interrupt Status: Interrupt Status of DMA Engine 11
#define DMA_INT12                                              0x00001000 // DMA Interrupt Status: Interrupt Status of DMA Engine 12
#define DMA_INT13                                              0x00002000 // DMA Interrupt Status: Interrupt Status of DMA Engine 13
#define DMA_INT14                                              0x00004000 // DMA Interrupt Status: Interrupt Status of DMA Engine 14
#define DMA_INT15                                              0x00008000 // DMA Interrupt Status: Interrupt Status of DMA Engine 15
#define DMA_EN0                                                0x00000001 // DMA Enable: Enable DMA Engine 0
#define DMA_EN1                                                0x00000002 // DMA Enable: Enable DMA Engine 1
#define DMA_EN2                                                0x00000004 // DMA Enable: Enable DMA Engine 2
#define DMA_EN3                                                0x00000008 // DMA Enable: Enable DMA Engine 3
#define DMA_EN4                                                0x00000010 // DMA Enable: Enable DMA Engine 4
#define DMA_EN5                                                0x00000020 // DMA Enable: Enable DMA Engine 5
#define DMA_EN6                                                0x00000040 // DMA Enable: Enable DMA Engine 6
#define DMA_EN7                                                0x00000080 // DMA Enable: Enable DMA Engine 7
#define DMA_EN8                                                0x00000100 // DMA Enable: Enable DMA Engine 8
#define DMA_EN9                                                0x00000200 // DMA Enable: Enable DMA Engine 9
#define DMA_EN10                                               0x00000400 // DMA Enable: Enable DMA Engine 10
#define DMA_EN11                                               0x00000800 // DMA Enable: Enable DMA Engine 11
#define DMA_EN12                                               0x00001000 // DMA Enable: Enable DMA Engine 12
#define DMA_EN13                                               0x00002000 // DMA Enable: Enable DMA Engine 13
#define DMA_EN14                                               0x00004000 // DMA Enable: Enable DMA Engine 14

//https://github.com/raspberrypi/firmware/issues/67
#define IRQ_BASE                                               (SUBSYSTEM_BASE + 0xB200)
#define IRQ_GPU_PENDING1                                       (SUBSYSTEM_BASE + 0xB204)
#define IRQ_GPU_PENDING2                                       (SUBSYSTEM_BASE + 0xB208)
#define IRQ_GPU_ENABLE2                                        (SUBSYSTEM_BASE + 0xB214)
#define IRQ_GPU_FAKE_ISR                                       0x10000

#define SMI_BASE                                               (SUBSYSTEM_BASE + 0x600000)
#define SMI_CS                                                 (SMI_BASE       + 0x00)

#define VCHIQ_BASE                                             (SUBSYSTEM_BASE + 0x0000B840)

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
#define FIRMWARE_VCHIQ_INIT   0x00048010
#define GET_COMMAND_LINE      0x00050001 // Config: Get Command Line (Response: ASCII Command Line String)
#define GET_DMA_CHANNELS      0x00060001 // Shared Resource Management: Get DMA Channels (DMA channels 0-15 Bits 16-31: reserved for future use)

#define PWM_BASE      (SUBSYSTEM_BASE + 0x20C000) // PWM Base Address
#define PWM_CTL       (PWM_BASE +      0x0  ) // PWM Control
#define PWM_STA       (PWM_BASE +      0x4  ) // PWM Status
#define PWM_DMAC      (PWM_BASE +      0x8  ) // PWM DMA Configuration
#define PWM_RNG1      (PWM_BASE +     0x10  ) // PWM Channel 1 Range
#define PWM_DAT1      (PWM_BASE +     0x14  ) // PWM Channel 1 Data
#define PWM_FIF1      (PWM_BASE +     0x18  ) // PWM FIFO Input
#define PWM_RNG2      (PWM_BASE +     0x20  ) // PWM Channel 2 Range
#define PWM_DAT2      (PWM_BASE +     0x24  ) // PWM Channel 2 Data

#define PWM_PWEN1     (/* PWM_BASE + */    0x1   ) // PWM Control: Channel 1 Enable
#define PWM_MODE1     (/* PWM_BASE + */    0x2   ) // PWM Control: Channel 1 Mode
#define PWM_RPTL1     (/* PWM_BASE + */    0x4   ) // PWM Control: Channel 1 Repeat Last Data
#define PWM_SBIT1     (/* PWM_BASE + */    0x8   ) // PWM Control: Channel 1 Silence Bit
#define PWM_POLA1     (/* PWM_BASE + */   0x10   ) // PWM Control: Channel 1 Polarity
#define PWM_USEF1     (/* PWM_BASE + */   0x20   ) // PWM Control: Channel 1 Use Fifo
#define PWM_CLRF1     (/* PWM_BASE + */   0x40   ) // PWM Control: Clear Fifo
#define PWM_MSEN1     (/* PWM_BASE + */   0x80   ) // PWM Control: Channel 1 M/S Enable
#define PWM_PWEN2     (/* PWM_BASE + */  0x100   ) // PWM Control: Channel 2 Enable
#define PWM_MODE2     (/* PWM_BASE + */  0x200   ) // PWM Control: Channel 2 Mode
#define PWM_RPTL2     (/* PWM_BASE + */  0x400   ) // PWM Control: Channel 2 Repeat Last Data
#define PWM_SBIT2     (/* PWM_BASE + */  0x800   ) // PWM Control: Channel 2 Silence Bit
#define PWM_POLA2     (/* PWM_BASE + */ 0x1000   ) // PWM Control: Channel 2 Polarity
#define PWM_USEF2     (/* PWM_BASE + */ 0x2000   ) // PWM Control: Channel 2 Use Fifo
#define PWM_MSEN2     (/* PWM_BASE + */ 0x8000   ) // PWM Control: Channel 2 M/S Enable
#define PWM_FULL1     (/* PWM_BASE + */    0x1   ) // PWM Status: Fifo Full Flag
#define PWM_EMPT1     (/* PWM_BASE + */    0x2   ) // PWM Status: Fifo Empty Flag
#define PWM_WERR1     (/* PWM_BASE + */    0x4   ) // PWM Status: Fifo Write Error Flag
#define PWM_RERR1     (/* PWM_BASE + */    0x8   ) // PWM Status: Fifo Read Error Flag
#define PWM_GAPO1     (/* PWM_BASE + */   0x10   ) // PWM Status: Channel 1 Gap Occurred Flag
#define PWM_GAPO2     (/* PWM_BASE + */   0x20   ) // PWM Status: Channel 2 Gap Occurred Flag
#define PWM_GAPO3     (/* PWM_BASE + */   0x40   ) // PWM Status: Channel 3 Gap Occurred Flag
#define PWM_GAPO4     (/* PWM_BASE + */   0x80   ) // PWM Status: Channel 4 Gap Occurred Flag
#define PWM_BERR      (/* PWM_BASE + */  0x100   ) // PWM Status: Bus Error Flag
#define PWM_STA1      (/* PWM_BASE + */  0x200   ) // PWM Status: Channel 1 State
#define PWM_STA2      (/* PWM_BASE + */  0x400   ) // PWM Status: Channel 2 State
#define PWM_STA3      (/* PWM_BASE + */  0x800   ) // PWM Status: Channel 3 State
#define PWM_STA4      (/* PWM_BASE + */ 0x1000   ) // PWM Status: Channel 4 State
#define PWM_ENAB      (0x80000000) // PWM DMA Configuration: DMA Enable

#define MAILBOX_MEM_FLAG_DISCARDABLE                                   (1 << 0)
#define MAILBOX_MEM_FLAG_NORMAL                                        (0 << 2)
#define MAILBOX_MEM_FLAG_DIRECT                                        (1 << 2)
#define MAILBOX_MEM_FLAG_COHERENT                                      (2 << 2)
#define MAILBOX_MEM_FLAG_L1_NONALLOCATING                              ((MAILBOX_MEM_FLAG_DIRECT | MAILBOX_MEM_FLAG_COHERENT))
#define MAILBOX_MEM_FLAG_ZERO                                          (1 << 4)
#define MAILBOX_MEM_FLAG_NO_INIT                                       (1 << 5)
#define MAILBOX_MEM_FLAG_HINT_PERMALOCK                                (1 << 6)

#endif
