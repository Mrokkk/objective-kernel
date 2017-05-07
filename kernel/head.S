#include <kernel/cpu/gdt.hpp>
#include <kernel/boot/multiboot.hpp>
#include <kernel/memory/paging/paging.hpp>

ALIGN(4)
SECTION(.multiboot)

#ifdef MULTIBOOT2
MULTIBOOT2_HEADER
#else
MULTIBOOT_HEADER
#endif

SECTION(.text.boot)

ENTRY(_start)

    mov $kernel_stack - KERNEL_PAGE_OFFSET, %esp
    pushl $0
    popfl
    push %eax
    push %ebx

    mov $KERNEL_DS, %eax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    movl $__bss_start - KERNEL_PAGE_OFFSET, %edi
    movl $__bss_end - KERNEL_PAGE_OFFSET, %ecx
    subl %edi, %ecx
    cld
    rep

    call read_bootloader_data

    mov $page0, %edi
    movl $PAGE_INIT_FLAGS, %eax
1:  stosl
    add $PAGE_SIZE, %eax
    cmp $__heap_start - KERNEL_PAGE_OFFSET + PAGE_INIT_FLAGS, %eax
    jne 1b

    mov $page_dir, %eax
    movl %eax, %cr3

    mov %cr0, %eax
    or $0x80000000, %eax
    mov %eax, %cr0
    mov $1f, %eax
    jmp *%eax

1:  mov $kernel_stack, %esp
    call main

1:  jmp 1b

ALIGN(PAGE_SIZE)
ENTRY(page_dir)
    .long page_table0
    .fill KERNEL_PAGE_OFFSET / 0x400000 - 1, 4, 0
    .long page_table0

ALIGN(PAGE_SIZE)
ENTRY(page0)
    .skip PAGE_INIT_FLAGS
page_table0:
    .skip PAGE_SIZE - PAGE_INIT_FLAGS

SECTION(.bss)

.skip 8192
ENTRY(kernel_stack)

