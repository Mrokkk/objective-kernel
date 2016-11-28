#include <drivers/serial.h>
#include <kernel/reboot.h>
#include <pointer.h>
#include <lib/ctype.h>
#include <stdarg.h>
#include <lib/cstring.h>
#include <kernel/memory/allocator.h>
#include <kernel/memory/heap_allocator.h>
#include <kernel/cpu/gdt.h>
#define TEST_MAIN
#include "tests.h"

int vsprintf(char *buf, const char *fmt, va_list args);

namespace tests {

int printf(const char *fmt, ...) {
    char printf_buf[512];
    va_list args;
    int printed;
    va_start(args, fmt);
    printed = vsprintf(printf_buf, fmt, args);
    va_end(args);
    drivers::serial::print(printf_buf);
    return printed;
}

char allocator_test_map[1024*1024];

TEST(allocator, can_allocate) {
    constexpr size_t memory_block_size = 32;
    auto expected = allocator_test_map + memory_block_size;
    kernel::allocator<kernel::heap_allocator, memory_block_size> test_allocator(allocator_test_map);
    for (auto i = 1; i < 1025; ++i) {
        auto result = test_allocator.allocate(i);
        REQUIRE(result == expected);
        *static_cast<unsigned int *>(result) = i;
        expected += memory_block_size * (2 + (i - 1) / memory_block_size);
    }
    for (unsigned int i = 1024; i > 0; --i) {
        expected -= memory_block_size * (2 + (i - 1) / memory_block_size);
        REQUIRE(*reinterpret_cast<unsigned int *>(expected) == i);
    }
}

TEST(allocator, cannot_free_invalid_ptr) {
    constexpr size_t memory_block_size = 32;
    kernel::allocator<kernel::heap_allocator, memory_block_size> test_allocator(allocator_test_map);
    for (auto i = 0; i < 4096; ++i) {
        REQUIRE(test_allocator.free(reinterpret_cast<void *>(i)));
    }
    for (auto i = 0; i < 100; ++i)
        test_allocator.allocate(i);
    for (auto i = 0; i < 4096; ++i) {
        REQUIRE(test_allocator.free(reinterpret_cast<void *>(i)));
    }
}

TEST(allocator, can_divide_blocks) {
    constexpr size_t memory_block_size = 32;
    kernel::allocator<kernel::heap_allocator, memory_block_size> test_allocator(allocator_test_map);
    auto data1 = test_allocator.allocate(120);
    auto expected = allocator_test_map + memory_block_size;
    REQUIRE(data1 == expected);
    test_allocator.free(data1);
    auto data2 = test_allocator.allocate(2);
    REQUIRE(data2 == expected);
    auto data3 = test_allocator.allocate(2);
    expected += memory_block_size * 2;
    REQUIRE(data3 == expected);
    auto data4 = test_allocator.allocate(2);
    expected += memory_block_size * 3; // dead memory area
    REQUIRE(data4 == expected);
    auto data5 = test_allocator.allocate(2);
    expected += memory_block_size * 2;
    REQUIRE(data5 == expected);
    auto data6 = test_allocator.allocate(2);
    expected += memory_block_size * 2;
    REQUIRE(data6 == expected);
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

void tests() {
    TEST_RUN(allocator, can_allocate);
    TEST_RUN(allocator, cannot_free_invalid_ptr);
    TEST_RUN(allocator, can_divide_blocks);
    TEST_RUN(kernel_allocator, can_allocate_and_free);
}

} // namespace tests

asmlinkage __noreturn void main() {
    cpu::gdt::initialize();
    auto lock = cpu::irq_save();
    drivers::serial::initialize();
    tests::tests();
    reboot();
    while (1);
}

