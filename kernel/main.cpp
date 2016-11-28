#include <drivers/vga.h>
#include <arch/reboot.h>
#include <pointer.h>
#include <stdarg.h>

asmlinkage char _end[];

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
    drivers::vga::init();
    printf("Hello World!\n");
    while (1);
}

