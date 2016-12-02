#include <kernel/memory/allocator.h>
#include <kernel/memory/heap_allocator.h>
#include "yatf/include/yatf.h"

char allocator_test_map[1024*1024];

TEST(allocator, can_allocate) {
    constexpr size_t memory_block_size = 32;
    auto expected = allocator_test_map + memory_block_size;
    memory::allocator<memory::heap_allocator, memory_block_size> test_allocator(allocator_test_map);
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
    memory::allocator<memory::heap_allocator, memory_block_size> test_allocator(allocator_test_map);
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
    memory::allocator<memory::heap_allocator, memory_block_size> test_allocator(allocator_test_map);
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

