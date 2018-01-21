#include <kernel/boot/multiboot.hpp>
#include <kernel/cpu/mmu.hpp>

ALIGN(4)
SECTION(.multiboot)

MULTIBOOT_HEADER()

SECTION(.boot)

ENTRY(_start)

    mov $kernel_stack - KERNEL_PAGE_OFFSET, %esp
    pushl $0
    popfl
    push %eax
    push %ebx

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
    cmp $0x100000 + PAGE_INIT_FLAGS, %eax
    jne 1b
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
    call global_constructors
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

.skip 4096 * 12
ENTRY(kernel_stack)

