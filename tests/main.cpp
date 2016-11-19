#include <drivers/serial.h>
#include <arch/reboot.h>
#include <pointer.h>
#include <lib/ctype.h>
#include <stdarg.h>
#include <lib/cstring.h>
#include <kernel/allocator.h>
#include <kernel/heap_allocator.h>

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


asmlinkage __noreturn void main() {
    serial_init();
    kernel::allocator<kernel::heap_allocator, 16> testAllocator1(testMap);
    auto result = testAllocator1.allocate(10);
    assert_eq(result, static_cast<void *>(testMap + 16));
    auto result2 = testAllocator1.free(result);
    assert(!result2);
    printf("all tests passed!\n");
    reboot();
    while (1);
}

} // namespace tests

