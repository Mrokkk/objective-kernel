#include <arch/asm.h>
#include <arch/multiboot.h>

#define FLAGS         MULTIBOOT_HEADER_FLAGS
#define MAGIC         MULTIBOOT_HEADER_MAGIC
#define CHECKSUM     -(MAGIC + FLAGS)

.align 4
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
.long 0x1badb002
.long 0x00000003
.long -(0x1badb002 + 0x00000003)
#endif
header_end:

.text
ENTRY(_start)

    mov $stack, %esp

    pushl $0
    popfl

    movl $_sbss, %edi
    movl $_ebss, %ecx
    subl %edi, %ecx
    cld
    rep

    call _init
    call main

1:
    jmp 1b

.skip 1024
stack:

