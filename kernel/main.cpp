#include <drivers/vga.h>
#include <kernel/reboot.h>
#include <kernel/cpu/gdt.h>
#include <kernel/cpu/idt.h>
#include <kernel/console.h>
#include <kernel/scheduler/process.h>
#include <pointer.h>

char user_stack[2048];

#define switch_to_user() \
    asm volatile(                           \
        "pushl %0;"                         \
        "mov %2, %%eax;"                    \
        "pushl %%eax;"                      \
        "pushl $0x0;"                       \
        "pushl %1;"                         \
        "push $1f;"                         \
        "iret;"                             \
        "1:"                                \
        "mov %0, %%eax;"                    \
        "mov %%ax, %%ds;"                   \
        "mov %%ax, %%es;"                   \
        "mov %%ax, %%fs;"                   \
        "mov %%ax, %%gs;"                   \
        :: "i" (cpu::segment::user_ds),     \
           "i" (cpu::segment::user_cs),     \
           "i" (&user_stack[2048])          \
    )

asmlinkage __noreturn void main() {
    cpu::gdt::initialize();
    cpu::idt::initialize();
    scheduler::initialize();
    drivers::vga::initialize();
    console::initialize(drivers::vga::print);
    console::print("Hello World!\n");
    switch_to_user();
    while (1);
}

