#pragma once

#include <kernel/cpu/mmu.hpp>

#define KERNEL_PAGE_OFFSET 0xc0000000

#ifndef __ASSEMBLER__

namespace memory {

struct mmap {
    enum class type {
        unknown,
        unavailable,
        available,
        device
    };
    type type;
    uint32_t base;
    uint32_t size;
};

extern struct mmap memory_map[];

template <typename T>
constexpr inline T align(T address, const uint32_t alignment) {
    const auto mod = (uint32_t)address % alignment;
    if (mod != 0) {
        return reinterpret_cast<T >((((uint32_t)address + alignment - 1) / 1024) * 1024);
    }
    return address;
}

template <typename T>
constexpr inline bool in_kernel_space(T address) {
    return reinterpret_cast<uint32_t>(address) > KERNEL_PAGE_OFFSET;
}

template <typename T>
constexpr inline T phys2virt(T address) {
    return reinterpret_cast<T>(reinterpret_cast<uint32_t>(address) + KERNEL_PAGE_OFFSET);
}

template <typename T>
constexpr inline T virt2phys(T address) {
    return reinterpret_cast<T>(reinterpret_cast<uint32_t>(address) - KERNEL_PAGE_OFFSET);
}

template <typename T>
constexpr inline uint32_t frame_number(T address) {
    return reinterpret_cast<uint32_t>(address) / PAGE_SIZE;
}

constexpr inline uint32_t frame_address(size_t index) {
    return index * PAGE_SIZE;
}

template <typename T>
constexpr inline uint32_t page_table_index(T address) {
    return PAGE_TABLES_NUMBER * (reinterpret_cast<uint32_t>(address) / PAGE_SIZE) / (((uint32_t)0 - 1) / PAGE_SIZE);
}

void initialize();

} // namespace memory

#endif

