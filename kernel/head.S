#include <kernel/multiboot.h>

.align 4
.section .multiboot
#ifdef MULTIBOOT2
MULTIBOOT2_HEADER
#else
MULTIBOOT_HEADER
#endif

.section .text.boot
ENTRY(_start)

    mov $stack, %esp

    pushl $0
    popfl

    movl $__bss_start, %edi
    movl $__bss_end, %ecx
    subl %edi, %ecx
    cld
    rep

    call _init
    call main

1:
    jmp 1b

.skip 1024
stack:

