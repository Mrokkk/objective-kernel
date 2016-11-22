#include <arch/multiboot.h>

#define FLAGS         MULTIBOOT_HEADER_FLAGS
#define MAGIC         MULTIBOOT_HEADER_MAGIC
#define CHECKSUM     -(MAGIC + FLAGS)

.section .multiboot
header_start:
#ifdef MULTIBOOT2
.long 0xe85250d6
.long 0
.long header_end - header_start
.long -(0xe85250d6 + 0 + (header_end - header_start))
.word 0
.word 0
.long 8
#else
.long MAGIC
.long FLAGS
.long CHECKSUM
#endif
header_end:

.text
.global _start
_start:

    movl $0x08, %eax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    mov $stack, %esp

    pushl $0
    popfl

    call _init
    call main

1:
    jmp 1b

.skip 1024
    stack:

