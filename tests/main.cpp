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

#define assert_eq(v1, v2) \
    { \
        if ((v1) != (v2)) printf("assertion failed: line %d: %d != %d\n", __LINE__, v1, v2); \
    }

#define assert_neq(v1, v2) \
    { \
        if ((v1) == (v2)) printf("assertion failed: line %d: %d == %d\n", __LINE__, v1, v2); \
    }

#define assert_gt(v1, v2) \
    { \
        if ((v1) <= (v2)) printf("assertion failed: line %d: %d != %d\n", __LINE__, v1, v2); \
    }

#define assert(cond) \
    { \
        if (!(cond)) printf("assertion failed: line %d\n", __LINE__); \
    } \

char testMap[1024*1024];

TEST(allocator, can_allocate) {
    kernel::allocator<kernel::heap_allocator, 32> testAllocator1(testMap);
    auto result = testAllocator1.allocate(10);
    ASSERT_TRUE(static_cast<void *>(result) == reinterpret_cast<void *>(static_cast<char *>(testMap) + 32));
    auto result2 = testAllocator1.free(result);
    assert(!result2);
}

asmlinkage __noreturn void main() {
    serial_init();
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
        assert_gt(reinterpret_cast<unsigned int>(c), reinterpret_cast<unsigned int>(b.get()));
        assert_gt(reinterpret_cast<unsigned int>(b.get()), reinterpret_cast<unsigned int>(a.get()));
    }
    allocator_can_allocate(&__allocator_can_allocate);
    printf("all tests passed!\n");
    reboot();
    while (1);
}

} // namespace tests

