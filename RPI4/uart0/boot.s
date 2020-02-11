.section ".text.boot"
.align 4
.global _start
.global _get_stack_pointer

_start:
	//TODO copy irq handler to 0x0
	msr DAIFSet, #0x0f
	mov sp, 0x80000
	bl main

_get_stack_pointer:
	mov x0, sp
	ret
