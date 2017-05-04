#include <allocator.h>
#include <algorithm.h>
#include <kernel/boot.hpp>
#include <kernel/memory/paging.hpp>
#include <kernel/memory/sections.hpp>
#include <kernel/memory/heap_allocator.hpp>

extern "C" char page_dir[];

void *operator new(size_t, void *address);

namespace memory {

char *_page_dir = page_dir + KERNEL_PAGE_OFFSET;
utils::allocator<heap_allocator, 32> *a = nullptr;
char *allocator_memory = nullptr;
uint32_t frames_size = 0u;
uint32_t *frames = nullptr;

template <typename Type, size_t size>
Type *align(Type *address) {
    auto mod = (uint32_t)address % size;
    if (mod != 0)
        return reinterpret_cast<Type *>((uint8_t *)address + mod);
    return address;
}

void initialize() {
    utils::fill(_page_dir, 4, 0);
    allocator_memory = sections::__heap_start;
    frames_size = 32768 * (boot::upper_mem / 1024) / 4096; // FIXME: align
    frames = new(align<char, 4096>(allocator_memory)) uint32_t;
    allocator_memory = align<char, 4096>(allocator_memory + frames_size);
    a = new(allocator_memory) utils::allocator<heap_allocator, 32>(align<char, 4096>(allocator_memory) + 64);
}

} // namespacce memory

