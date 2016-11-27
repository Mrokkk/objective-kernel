#include <drivers/vga.h>
#include <arch/reboot.h>
#include <pointer.h>

asmlinkage char _end[];

int printf(const char *fmt, ...);

asmlinkage __noreturn void main() {
    cpu::gdt::initialize();
    video_init();
    printf("Hello World!\n");
    while (1);
}

