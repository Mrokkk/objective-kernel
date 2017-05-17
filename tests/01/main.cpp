#define YATF_MAIN
#include <yatf.hpp>
#include <cstdio>

void atomic_increment(void *addr) {
    asm volatile("lock incl (%0)" :: "r" (addr));
}

void atomic_decrement(void *addr) {
    asm volatile("lock decl (%0)" :: "r" (addr));
}

void console_init();

int main(int argc, const char *argv[]) {
    console_init();
    return yatf::main(printf, argc, argv);
}

