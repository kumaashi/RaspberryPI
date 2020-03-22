.section ".text.boot"
.align 4
.global _start
.global _get_stack_pointer
.global _invalidate_data_cache
.global _memory_barrier
.global _cpu_nop

_start:
	//TODO copy irq handler to 0x0
	MSR DAIFSet, #0x0f
	MOV sp, 0x80000
	BL main

_get_stack_pointer:
	mov x0, sp
	ret

_memory_barrier:
	dmb ish
	dmb ishst
	dmb ishld
	ret

_cpu_nop:
	nop
	ret

_invalidate_data_cache:
	MOV X0, #0x0 // X0 = Cache level
	MSR CSSELR_EL1, x0 // 0x0 for L1 Dcache 0x2 for L2 Dcache.
	MRS X4, CCSIDR_EL1 // Read Cache Size ID.
	AND X1, X4, #0x7
	ADD X1, X1, #0x4 // X1 = Cache Line Size.
	LDR X3, =0x7FFF
	AND X2, X3, X4, LSR #13 // X2 = Cache Set Number ? 1.
	LDR X3, =0x3FF
	AND X3, X3, X4, LSR #3 // X3 = Cache Associativity Number ? 1.
	CLZ W4, W3 // X4 = way position in the CISW instruction.
	MOV X5, #0 // X5 = way counter way_loop.
way_loop:
	MOV X6, #0 // X6 = set counter set_loop.
set_loop:
	LSL X7, X5, X4
	ORR X7, X0, X7 // Set way.
	LSL X8, X6, X1
	ORR X7, X7, X8 // Set set.
	DC cisw, X7 // Clean and Invalidate cache line.
	ADD X6, X6, #1 // Increment set counter.
	CMP X6, X2 // Last set reached yet?
	BLE set_loop // If not, iterate set_loop,
	ADD X5, X5, #1 // else, next way.
	CMP X5, X3 // Last way reached yet?
	BLE way_loop // If not, iterate way_loop.
	RET
