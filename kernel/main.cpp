#include <basic/linkage.h>
#include <drivers/vga.h>
#include <arch/reboot.h>
#include <pointer.h>

#define __noreturn __attribute__((noreturn))

asmlinkage char _end[];

asmlinkage __noreturn void main() {
    video_init();
    auto a = yacppl::make_shared<int>(1);
    auto b = yacppl::make_unique<char>(2);
    auto c = new int{4};
    delete c;
    display_print("Hello World!");
#ifdef CI
    reboot();
#endif
    while (1);
}

