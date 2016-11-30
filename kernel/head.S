#include <kernel/multiboot.h>
#include <kernel/cpu/gdt.h>
#include <kernel/memory/paging.h>

.align 4
.section .multiboot
#ifdef MULTIBOOT2
MULTIBOOT2_HEADER
#else
MULTIBOOT_HEADER
#endif

.section .text.boot
ENTRY(_start)

    pushl $0
    popfl

    mov $page0, %edi
    movl $PAGE_INIT_FLAGS, %eax
1:  stosl
    add $0x1000, %eax
    cmp $__heap_start + PAGE_INIT_FLAGS, %eax
    jne 1b

    mov $kernel_stack, %esp

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

ENTRY(kernel_stack)

.align 0x1000
ENTRY(page_dir)
    /*
     * First entry will be zeroed just before
     * the call to the page_dir_reload
     */
    .long page_table0
    .fill KERNEL_PAGE_OFFSET / 0x400000 - 1, 4, 0
    .long page_table0

.align 0x1000
ENTRY(page0)
    .skip PAGE_INIT_FLAGS
page_table0:
    .skip 0x1000 - PAGE_INIT_FLAGS

