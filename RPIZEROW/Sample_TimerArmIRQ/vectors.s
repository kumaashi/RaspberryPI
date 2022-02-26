.org 0x0000

b  _start     //0x00 Reset/Boot
bl 0x00000000 //0x04 Undefined instruction
bl 0x00000000 //0x08 Supervisor call
bl 0x00000000 //0x0C Prefetch abort
bl 0x00000000 //0x10 Data abort
bl 0x00000000 //0x14 Hyp trap
b  intr_irq   //0x18 IRQ interrupt
bl 0x00000000 //0x1C FIQ interrupt

.org 0x8000
.globl _start
_start:
    mov sp,#0x8000
    bl notmain
hang:
  	b hang

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
    bx lr

.globl intr_irq
intr_irq:
    mov sp,#0x4000
		str lr, [sp, #0x4]
		bl fn_intr_irq
		ldr lr, [sp, #0x4]
		subs pc, lr,#4

