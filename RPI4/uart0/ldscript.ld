OUTPUT_ARCH(aarch64)

ENTRY(_start)

MEMORY
{
	/* Raspberry Pi has 1GiB(0x40000000) RAM */
	/* entry point is 0x80000 */
	RAM(xrw) : ORIGIN = 0x80000 , LENGTH = (0x40000000 - 0x80000)
}

SECTIONS
{
	.text : {
		__text_start = .;
		__start = .;
		KEEP(*(.text.boot))
		*(.text*)
		. = ALIGN(4096);
		__text_end = .;
	} > RAM

	.rodata : {
		__rodata_start = .;
		*(.rodata*)
		. = ALIGN(4096);
		__rodata_end = .;
	} > RAM

	.data : {
		__data_start = . ;
		*(.data*)
		. = ALIGN(4096);
		__data_end = . ;
	} > RAM

	.bss : {
		__bss_start = . ;
		*(.bss*)
		. = ALIGN(4096);
		__bss_end = . ;
	} > RAM

	.heap : {
		__heap_start = .;
		/* 64MiB heap */
		. = . + 1024 * 1024 * 64;
		. = ALIGN(4096);
		__heap_end = .;
	} > RAM

	.stack : {
		__stack_end = .;
		/* 64MiB space */
		__stack_start = __stack_end + (1024*1024) * 64;
	} > RAM
}
