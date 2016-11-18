#include <basic/definitions.h>
#include <drivers/vga.h>
#include <arch/reboot.h>
#include <pointer.h>

asmlinkage char _end[];

int printf(const char *fmt, ...);

asmlinkage __noreturn void main() {
    video_init();
    printf("Hello World!\n");
#ifdef CI
    reboot();
#endif
    while (1);
}

