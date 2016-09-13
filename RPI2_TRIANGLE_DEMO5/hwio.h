#ifndef _HWIO_H_
#define _HWIO_H_

#include "mailbox.h"

#define IO_READ(reg)                                          (*(volatile uint32_t *)((uint32_t)reg))
#define IO_WRITE(reg, val)                                    (*(volatile uint32_t *)((uint32_t)reg) = (uint32_t)val)

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
#define MAILBOX_WRITE                                         (MAILBOX_BASE   + 0x20)

#define MAILBOX_FULL                                          0x80000000
#define MAILBOX_EMPTY                                         0x40000000
#define MAILBOX_FRAMEBUFFER                                   1
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

uint32_t io_read(uint32_t);
void io_write(uint32_t, uint32_t);
void SetupCache();
void DiscardDataCache();
void MemoryBarrier();
void FlushCache();
void dmb();

#endif
