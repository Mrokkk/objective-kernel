#include "segment.hpp"

#define EIP_PUSHED      0
#define CS_PUSHED       4
#define EFLAGS_PUSHED   8
#define ESP_PUSHED      12
#define SS_PUSHED       16

#define SAVE_ALL \
    pushl %gs;          /* gs */  \
    pushl %fs;          /* fs */  \
    pushl %es;          /* es */  \
    pushl %ds;          /* ds */  \
    push %eax;          /* eax */ \
    mov $KERNEL_DS, %eax;         \
    mov %ax, %ds;                 \
    mov %ax, %es;                 \
    mov %ax, %fs;                 \
    mov (%esp), %eax;             \
    push %ebp;          /* ebp */ \
    push %edi;          /* edi */ \
    push %esi;          /* esi */ \
    push %edx;          /* edx */ \
    push %ecx;          /* ecx */ \
    push %ebx;          /* ebx */

#define RESTORE_ALL \
    pop %ebx; \
    pop %ecx; \
    pop %edx; \
    pop %esi; \
    pop %edi; \
    pop %ebp; \
    pop %eax; \
    popl %ds; \
    popl %es; \
    popl %fs; \
    popl %gs;

#define PIC_EOI(nr) \
    mov $0x20, %al; \
    .if nr > 7; \
        outb %al, $0xa0; \
    .endif; \
    outb %al, $0x20

#define pic_isr(x) \
    ENTRY(isr_##x) \
        SAVE_ALL; \
        push %esp; \
        push $x-0x20; \
        call do_irq; \
        PIC_EOI(x); \
        add $8, %esp; \
        RESTORE_ALL; \
        iret; \

#define exception_noerrno(x) \
    ENTRY(exc_##x##_handler) \
        SAVE_ALL; \
        push $0; \
        push $__NR_##x; \
        call exception_handler; \
        add $12, %esp; \
        RESTORE_ALL; \
        iret;

#define exception_errno(x) \
    ENTRY(exc_##x##_handler) \
        mov (%esp), %eax; \
        add $4, %esp; \
        SAVE_ALL; \
        push %eax; \
        push $__NR_##x; \
        call exception_handler; \
        add $12, %esp; \
        RESTORE_ALL; \
        iret;

#include "irqs.hpp"
#include "exceptions.hpp"

ENTRY(systick_handler)
    incl jiffies
    SAVE_ALL
    call do_scheduler
    PIC_EOI(0)
    RESTORE_ALL
    iret

ENTRY(syscall_handler)
    iret

