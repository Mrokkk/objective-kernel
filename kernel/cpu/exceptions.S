#include "asm.h"
#include "gdt.hpp"

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

#include "irqs.h"
#include "exceptions.h"

ENTRY(systick_handler)
    incl jiffies
    SAVE_ALL
    PIC_EOI(0)
    RESTORE_ALL
    iret

ENTRY(syscall_handler)
    iret

