ENTRY(_start)

__kernel_phys = 0x00100000;
__kernel_virt = 0xC0100000;

__kernel_offset = __kernel_virt - __kernel_phys;

SECTIONS {

    . = __kernel_phys;

    .unpagged BLOCK(4K) : ALIGN(4K) {
        __unpagged_start = .;
        KEEP(*(.multiboot))
        KEEP(*(.text.boot))
        __unpagged_end = .;
    }

    . += __kernel_offset;

    . = ALIGN(4K);
    .text BLOCK(4K) : AT(ADDR(.text) - __kernel_offset) {
        __text_start = .;
        *(.text)
        __text_end = .;
    }

    . = ALIGN(4K);
    .rodata BLOCK(4K) : AT(ADDR(.rodata) - __kernel_offset) {
        *(.rodata)
    }

    .init_array : AT(ADDR(.init_array) - __kernel_offset) {
        __init_array_start = .;
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP (*(.init_array .ctors))
        __init_array_end = .;
    }

    . = ALIGN(4K);
    .data BLOCK(4K) : AT(ADDR(.data) - __kernel_offset) {
        __data_start = .;
        *(.data)
        __data_end = .;
    }

    . = ALIGN(4K);
    .bss BLOCK(4K) : AT(ADDR(.bss) - __kernel_offset) {
        __bss_start = .;
        *(COMMON)
        *(.bss)
        *(.stack)
        __bss_end = .;
    }

    . = ALIGN(4K);

    __heap_start = .;

}

