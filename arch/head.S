#include <basic/multiboot.h>

#define FLAGS         MULTIBOOT_HEADER_FLAGS
#define MAGIC         MULTIBOOT_HEADER_MAGIC
#define CHECKSUM     -(MAGIC + FLAGS)

.section .multiboot
.long MAGIC
.long FLAGS
.long CHECKSUM

.text
.global _start
_start:

    pushl $0
    popfl

    movl $0x08, %eax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    mov $stack, %esp

    call _init
    call main

1:
    jmp 1b

.skip 1024
    stack:



