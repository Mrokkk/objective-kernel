#include <basic/linkage.h>
#include <drivers/vga.h>
#include <arch/reboot.h>
#include <pointer.h>

#define __noreturn __attribute__((noreturn))

asmlinkage char _end[];

int printf(const char *fmt, ...);

asmlinkage __noreturn void main() {
    video_init();
    printf("Hello World!\n");
    for (int i = 0; i < 3; i++) {
        auto a = yacppl::make_shared<int>(1);
        auto b = yacppl::make_unique<char>(2);
        auto c = new int{498};
        printf("a = %d [0x%x]\n", *a, reinterpret_cast<unsigned int>(a.get()));
        printf("b = %d [0x%x]\n", *b, reinterpret_cast<unsigned int>(b.get()));
        printf("c = %d [0x%x]\n", *c, reinterpret_cast<unsigned int>(c));
        delete c;
        auto d = new int(5);
        printf("c = %d [0x%x]\n", *c, reinterpret_cast<unsigned int>(c));
        printf("d = %d [0x%x]\n", *d, reinterpret_cast<unsigned int>(d));
        delete d;
    }
#ifdef CI
    reboot();
#endif
    while (1);
}

