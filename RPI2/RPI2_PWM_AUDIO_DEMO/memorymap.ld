ENTRY(_start)

SECTIONS
{
    /* Starts at LOADER_ADDR. */
    . = 0x8000;
    .text.boot :
    {
        . = ALIGN(4);
        *(.text.boot)
        . = ALIGN(4);
    }

    .text :
    {
        . = ALIGN(4096);
        *(.text)
        . = ALIGN(4096);
    }

    .rodata : 
    {
        . = ALIGN(1);
        *(.rodata*)
        . = ALIGN(1);
    }

    .data :
    {
        . = ALIGN(4096);
        *(.data)
        . = ALIGN(4096);
    }

    .bss : 
    {
        . = ALIGN(4096);
        *(.bss)
        . = ALIGN(4096);
    }
}

