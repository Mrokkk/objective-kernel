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

    /* Set segment registers */
    mov $KERNEL_DS, %eax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    mov $page0, %edi
    movl $PAGE_INIT_FLAGS, %eax
1:
    stosl
    add $0x1000, %eax                        //FIXME
    cmp $__heap_start - KERNEL_PAGE_OFFSET + 0x10000 + PAGE_INIT_FLAGS, %eax
    jne 1b

    mov $page_dir, %eax
    movl %eax, %cr3

    mov %cr0, %eax
    or $0x80000000, %eax
    mov %eax, %cr0
    mov $1f, %eax
    jmp *%eax

1:
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


.align 0x1000
ENTRY(page_dir)
    .long page_table0
    .fill KERNEL_PAGE_OFFSET / 0x400000 - 1, 4, 0
    .long page_table0

.align 0x1000
ENTRY(page0)
    .skip PAGE_INIT_FLAGS
page_table0:
    .skip 0x1000 - PAGE_INIT_FLAGS

.section .bss

.skip 2048
ENTRY(kernel_stack)


