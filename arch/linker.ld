ENTRY(_start)

SECTIONS
{

    . = 1M;

    .text BLOCK(4K) : ALIGN(4K)
    {
        _stext = .;
        KEEP(*(.multiboot))
        *(.text)
        _etext = .;
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .preinit_array :
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    }

    .init_array :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
        PROVIDE_HIDDEN (__init_array_end = .);
    }

    .fini_array :
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
        KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
        PROVIDE_HIDDEN (__fini_array_end = .);
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        _sdata = .;
        *(.data)
        _smodules_data = .;
        KEEP(*(.modules_data))
        _emodules_data = .;
        _edata = .;
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        _sbss = _edata;
        *(COMMON)
        *(.bss)
        *(.stack)
        _ebss = .;
    }

    . = ALIGN(4K);

    _end = .;

}

