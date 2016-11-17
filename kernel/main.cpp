#include <basic/linkage.h>
#include <drivers/vga.h>
#include <arch/reboot.h>
#include <pointer.h>

#define __noreturn __attribute__((noreturn))

asmlinkage char _end[];

int printf(const char *fmt, ...);

#define assert_eq(v1, v2) \
    { \
        if ((v1) != (v2)) printf("assertion failed: line %d: %d != %d\n", __LINE__, v1, v2); \
    }

#define assert_neq(v1, v2) \
    { \
        if ((v1) == (v2)) printf("assertion failed: line %d: %d != %d\n", __LINE__, v1, v2); \
    }

asmlinkage __noreturn void main() {
    video_init();
    printf("Hello World!\n");
    for (int i = 0; i < 1024; i++) {
        auto a = yacppl::make_shared<int>(1);
        assert_eq(*a, 1);
        auto b = yacppl::make_unique<char>(2);
        assert_eq(*b, 2);
        assert_neq(reinterpret_cast<unsigned int>(b.get()), reinterpret_cast<unsigned int>(a.get()));
        auto c = new int{498};
        assert_eq(*c, 498);
        assert_neq(reinterpret_cast<unsigned int>(c), reinterpret_cast<unsigned int>(a.get()));
        assert_neq(reinterpret_cast<unsigned int>(c), reinterpret_cast<unsigned int>(b.get()));
        assert_eq(*a, 1);
        assert_eq(*b, 2);
        delete c;
        auto d = new int(5);
        delete d;
        assert_eq(c, d);
    }
    printf("all tests passed!");
#ifdef CI
    reboot();
#endif
    while (1);
}

