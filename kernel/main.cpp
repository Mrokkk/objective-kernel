#include <drivers/vga.h>
#include <kernel/reboot.h>
#include <kernel/cpu/gdt.h>
#include <kernel/cpu/idt.h>
#include <kernel/console.h>
#include <pointer.h>

asmlinkage __noreturn void main() {
    cpu::gdt::initialize();
    cpu::idt::initialize();
    drivers::vga::initialize();
    console::initialize(drivers::vga::print);
    console::print("Hello World!\n");
    while (1);
}

