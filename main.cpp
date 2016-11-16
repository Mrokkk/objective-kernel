#include <linkage.h>
#include <vga.h>

#define __noreturn __attribute__((noreturn))

asmlinkage __noreturn void main()
{
    video_init();
    display_print("Hello World!");
    while (1);
}

