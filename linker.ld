ENTRY(_start)

SECTIONS {

    . = 1M;

    .unpagged BLOCK(4K) : ALIGN(4K) {
        __unpagged_start = .;
        KEEP(*(.multiboot))
        KEEP(*(.text.boot))
        __unpagged_end = .;
    }

    .text BLOCK(4K) : ALIGN(4K) {
        __text_start = .;
        *(.text)
        __text_end = .;
    }

    .rodata BLOCK(4K) : ALIGN(4K) {
        *(.rodata)
    }

    .preinit_array : {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    }

    .init_array : {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
        PROVIDE_HIDDEN (__init_array_end = .);
    }

    .fini_array : {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
        KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
        PROVIDE_HIDDEN (__fini_array_end = .);
    }

    .data BLOCK(4K) : ALIGN(4K) {
        __data_start = .;
        *(.data)
        __data_end = .;
    }

    .bss BLOCK(4K) : ALIGN(4K) {
        __bss_start = .;
        *(COMMON)
        *(.bss)
        *(.stack)
        __bss_end = .;
    }

    . = ALIGN(4K);

    __heap_start = .;

}

