#pragma once

#include <kernel/cpu/mmu.hpp>

namespace memory {

template <typename Type>
inline Type align(Type address, uint32_t alignment) {
    auto mod = (uint32_t)address % alignment;
    if (mod != 0) {
        return reinterpret_cast<Type >((((uint32_t)address + alignment - 1) / 1024) * 1024);
    }
    return address;
}

template <typename Type>
constexpr inline Type virt_address(Type address) {
    return reinterpret_cast<Type>(reinterpret_cast<uint32_t>(address) + KERNEL_PAGE_OFFSET);
}

template <typename Type>
constexpr inline Type phys_address(Type address) {
    return reinterpret_cast<Type>(reinterpret_cast<uint32_t>(address) - KERNEL_PAGE_OFFSET);
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

