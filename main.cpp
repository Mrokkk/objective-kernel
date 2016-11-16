#include <linkage.h>
#include <vga.h>
#include <reboot.h>

#define __noreturn __attribute__((noreturn))

asmlinkage __noreturn void main() {
    video_init();
    display_print("Hello World!");
#ifdef CI
    reboot();
#endif
    while (1);
}

