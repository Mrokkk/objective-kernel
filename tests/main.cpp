#include <drivers/serial.h>
#include <kernel/reboot.h>
#include <pointer.h>
#include <stdarg.h>
#include <lib/cstring.h>
#include <kernel/cpu/gdt.h>

#define TEST_MAIN
#include "tests.h"

int vsprintf(char *buf, const char *fmt, va_list args);

int tprintf(const char *fmt, ...) {
    char printf_buf[512];
    va_list args;
    int printed;
    va_start(args, fmt);
    printed = vsprintf(printf_buf, fmt, args);
    va_end(args);
    drivers::serial::print(printf_buf);
    return printed;
}

TEST(kernel_allocator, can_allocate_and_free) {
    for (int i = 0; i < 1024; i++) {
        auto a = yacppl::make_shared<int>(1);
        REQUIRE(*a == 1);
        auto b = yacppl::make_unique<char>(2);
        REQUIRE(*b == 2);
        REQUIRE(reinterpret_cast<unsigned int>(b.get()) != reinterpret_cast<unsigned int>(a.get()));
        auto c = new int{498};
        REQUIRE(*c == 498);
        REQUIRE(reinterpret_cast<unsigned int>(c) != reinterpret_cast<unsigned int>(a.get()));
        REQUIRE(reinterpret_cast<unsigned int>(c) != reinterpret_cast<unsigned int>(b.get()));
        REQUIRE(*a == 1);
        REQUIRE(*b == 2);
        delete c;
        auto d = new int(5);
        delete d;
        REQUIRE(c == d);
        REQUIRE(reinterpret_cast<unsigned int>(c) > reinterpret_cast<unsigned int>(b.get()));
        REQUIRE(reinterpret_cast<unsigned int>(b.get()) > reinterpret_cast<unsigned int>(a.get()));
    }
}

asmlinkage __noreturn void main() {
    cpu::gdt::initialize();
    auto lock = cpu::irq_save();
    drivers::serial::initialize();
    etf::main();
    reboot();
    while (1);
}

