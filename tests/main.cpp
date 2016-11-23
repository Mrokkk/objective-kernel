#include <drivers/serial.h>
#include <arch/reboot.h>
#include <pointer.h>
#include <lib/ctype.h>
#include <stdarg.h>
#include <lib/cstring.h>
#include <kernel/allocator.h>
#include <kernel/heap_allocator.h>
#include "tests.h"

asmlinkage char _end[];

int vsprintf(char *buf, const char *fmt, va_list args);

namespace tests {

int printf(const char *fmt, ...) {
    char printf_buf[512];
    va_list args;
    int printed;
    va_start(args, fmt);
    printed = vsprintf(printf_buf, fmt, args);
    va_end(args);
    serial_print(printf_buf);
    return printed;
}

char testMap[1024*1024];

TEST(allocator, can_allocate) {
    kernel::allocator<kernel::heap_allocator, 32> testAllocator1(testMap);
    auto result = testAllocator1.allocate(10);
    REQUIRE(static_cast<void *>(result) == reinterpret_cast<void *>(static_cast<char *>(testMap) + 32));
    auto result2 = testAllocator1.free(result);
    REQUIRE_FALSE(result2);
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
    serial_init();
    TEST_RUN(allocator, can_allocate);
    TEST_RUN(kernel_allocator, can_allocate_and_free);
    reboot();
    while (1);
}

} // namespace tests

