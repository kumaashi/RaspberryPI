.section ".text.boot"
.text
.align  4
.global _start
_start:
	
	// https://github.com/PeterLemon/RaspberryPi/blob/master/NEON/Fractal/Julia/kernel7.asm
	mov r0,  #0 
	mov r1,  #0 
	mov r2,  #0 
	mov r3,  #0 
	mov r4,  #0 
	mov r5,  #0 
	mov r6,  #0 
	mov r7,  #0 
	mov r8,  #0 
	mov r9,  #0 
	mov r10, #0
	mov r11, #0
	
	mrc p15,0,r0,c1,c0,0         // L1 cache
	orr r0,#0x0004               // Data Cache (Bit 2)
	orr r0,#0x0800               // Branch Prediction (Bit 11)
	orr r0,#0x1000               // Instruction Caches (Bit 12)
	mcr p15,0,r0,c1,c0,0         // System Control Register = R0
	
	mrc p15,0,r0,c1,c0,2         // R0 = Access Control Register
	orr r0, #0x300000            // Enable Single & Double Precision
	add r0, #0xC00000
	mcr p15,0,r0,c1,c0,2         // Access Control Register = R0
	mov r0,#0x40000000           // R0 = Enable VFP
	vmsr fpexc,r0                // FPEXC = R0

	#per Core 
	mrc p15,0,r0,c0,c0,5         // core check 
	cmp  r0,#3                   // 
	beq  _looping                // 
	cmp  r0,#2                   // 
	beq  _looping                // 
	cmp  r0,#1                   // 
	beq  _core1_boot

	//ands r0,#3                   // 
	//bne _looping                 // Other Core Loop

	mov r13, #0x8000
	mov r0,  #0x000000d3
	msr cpsr, r0
	b main
_core1_boot:
	//mov r13, #0x4000
	//mov r0,  #0x000000d3
	//msr cpsr, r0
	//b core1_update
_looping:
	b _looping


.global DataMemoryBarrierAll
DataMemoryBarrierAll:
	mov r0, #0                   //  The read register Should Be Zero before the call
	mcr p15, 0, r0, C7, C6, 0    //  Invalidate Entire Data Cache
	mcr p15, 0, r0, c7, c10, 0   //  Clean Entire Data Cache
	mcr p15, 0, r0, c7, c14, 0   //  Clean and Invalidate Entire Data Cache
	mcr p15, 0, r0, c7, c10, 4   //  Data Synchronization Barrier
	mcr p15, 0, r0, c7, c10, 5   //  Data Memory Barrier
	mov pc, lr


.global MemoryBarrier
MemoryBarrier:
	mcr p15, #0, ip, c7, c5,  0 // invalidate I cache
	mcr p15, #0, ip, c7, c5,  6 // invalidate BTB
	mcr p15, #0, ip, c7, c10, 4 // drain write buffer
	mcr p15, #0, ip, c7, c5,  4 // prefetch flush
	mov pc, lr

.global SetupCache
SetupCache:
	mov r0, #0
	mcr p15, 0, r0, c7, c7, 0     // Invalidate caches
	mcr p15, 0, r0, c8, c7, 0     // Invalidate TLB
	mrc p15, 0, r0, c1, c0, 0     //
	ldr r1, =0x1004               //
	orr r0, r0, r1                // Set L1 enable bit
	mcr p15, 0, r0, c1, c0, 0
	mov pc, lr

.global DiscardDataCache
DiscardDataCache:
	mov r0, #0
	mcr p15, 0, r0, c7, c6, 0
	mov pc, lr

.global DataMemoryBarrier
DataMemoryBarrier:
	mov r0, #0
	mcr p15, #0, r0, c7, c10, #5
	mov pc, lr

.global DataSyncBarrier
DataSyncBarrier:
	mov r0, #0
	mcr p15, #0, r0, c7, c10, #4
	mov pc, lr

.global FlushCache
FlushCache:
	mov r0, #0
	mcr p15, #0, r0, c7, c10, #0
	mov pc, lr


.global reset_jump
reset_jump:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	mov r4, #0
	mov pc, r0


