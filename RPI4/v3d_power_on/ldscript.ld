ENTRY(_start)

SECTIONS
{
	. = 0x80000;
	__start = .;
	__text_start = .;
	.text : {
		KEEP(*(.text.boot))
		main.o
		*(.text)
	}
	.rodata : 
	{
		*(.strings)
		*(.rodata)
		*(.rodata.*)
	}

	.data : { *(.data) }
	.bss : {
		*(.bss)
	}
}

