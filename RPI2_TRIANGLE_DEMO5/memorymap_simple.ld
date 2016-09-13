ENTRY(_start)
 
SECTIONS
{
    /* Starts at LOADER_ADDR. */
    . = 0x8000;
    __start = .;
    __text_start = .;
    . = ALIGN(4096); /* align to page size */
    .text :
    {
        *(.text.boot)
        *(.text)
        *(.data)
        *(.bss)
        *(.rodata)
    }
    __text_end = .;
    __end = .;
}

