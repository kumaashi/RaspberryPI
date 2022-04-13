.org 0x0000
b _START     //0x00 Reset/Boot
b HANG       //0x04 Undefined instruction
b HANG       //0x08 Supervisor call
b HANG       //0x0C Prefetch abort
b HANG       //0x10 Data abort
b HANG       //0x14 Hyp trap
b INTR_IRQ   //0x18 IRQ interrupt
b HANG       //0x1C FIQ interrupt

.org 0x8000
.globl _START
_START:
	mov sp,#0x8000

	//todo 
	//mrc p15,0,r0,c1,c0,0         // L1 cache
	//orr r0,#0x0004               // Data Cache (Bit 2)
	//orr r0,#0x0800               // Branch Prediction (Bit 11)
	//orr r0,#0x1000               // Instruction Caches (Bit 12)
	//mcr p15,0,r0,c1,c0,0         // System Control Register = R0

	bl notmain
HANG:
	bl handle_hang
	b HANG

.globl ENABLE_IRQ
ENABLE_IRQ:
	mrs r0, cpsr
	bic r0, r0, #0x80
	msr cpsr_c, r0
	bx lr

.globl DISABLE_IRQ
DISABLE_IRQ:
	mrs r0, cpsr
	orr r0, r0, #0x80
	msr cpsr_c, r0
	bx lr

.globl PUT32
PUT32:
	str r1,[r0]
	bx lr

.globl GET32
GET32:
	ldr r0,[r0]
	bx lr

.globl SLEEP
SLEEP:
	subs r0, #0x1
	bne SLEEP
	bx lr

.globl INTR_IRQ
INTR_IRQ:
	mov sp,#0x4000
	str lr, [sp, #0x4]
	bl intr_handler
	ldr lr, [sp, #0x4]
	subs pc, lr,#4

	/*
	subs pc, lr,#4
	//mov sp, #0x4000
	//bl intr_handler
	//bx lr
	*/

.global FLUSH_CACHE
FLUSH_CACHE:
	mov r0, #0                   //  The read register Should Be Zero before the call
	mcr p15, 0, r0, C7, C6, 0    //  Invalidate Entire Data Cache
	mcr p15, 0, r0, c7, c10, 0   //  Clean Entire Data Cache
	mcr p15, 0, r0, c7, c14, 0   //  Clean and Invalidate Entire Data Cache
	mcr p15, 0, r0, c7, c10, 4   //  Data Synchronization Barrier
	mcr p15, 0, r0, c7, c10, 5   //  Data Memory Barrier
	mov pc, lr

.global MemoryBarrier
MemoryBarrier:
	mcr p15, #0, r0, c7, c5,  0 // invalidate I cache
	mcr p15, #0, r0, c7, c5,  6 // invalidate BTB
	mcr p15, #0, r0, c7, c10, 4 // drain write buffer
	mcr p15, #0, r0, c7, c5,  4 // prefetch flush
	mov pc, lr


.global InvalidateData
InvalidateData:
	mov r0, #0                   // The read register Should Be Zero before the call
	mcr p15, 0, r0, C7, C6, 0    // Invalidate Entire Data Cache
	mcr p15, 0, r0, c7, c10, 0   // Clean Entire Data Cache
	mcr p15, 0, r0, c7, c14, 0   // Clean and Invalidate Entire Data Cache
	mcr p15, 0, r0, c7, c10, 4   // Data Synchronization Barrier
	mcr p15, 0, r0, c7, c10, 5   // Data Memory Barrier
	mcr p15, 0, r0, c7, c5,  0   // invalidate I cache
	mcr p15, 0, r0, c7, c5,  6   // invalidate BTB
	mcr p15, 0, r0, c7, c10, 4   // drain write buffer
	mcr p15, 0, r0, c7, c5,  4   // prefetch flush
	mov pc, lr

