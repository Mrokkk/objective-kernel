#include <drivers/vga.h>
#include <kernel/reboot.h>
#include <kernel/cpu/gdt.h>
#include <kernel/cpu/idt.h>
#include <pointer.h>
#include <stdarg.h>

int vsprintf(char *buf, const char *fmt, va_list args);

int printf(const char *fmt, ...) {
    char printf_buf[512];
    va_list args;
    int printed;
    va_start(args, fmt);
    printed = vsprintf(printf_buf, fmt, args);
    va_end(args);
    drivers::vga::print(printf_buf);
    return printed;
}

asmlinkage __noreturn void main() {
    cpu::gdt::initialize();
    cpu::idt::initialize();
    drivers::vga::initialize();
    printf("Hello World!\n");
    while (1);
}

