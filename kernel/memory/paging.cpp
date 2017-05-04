#include <allocator.h>
#include <algorithm.h>
#include <kernel/memory/paging.hpp>
#include <kernel/memory/sections.hpp>
#include <kernel/memory/heap_allocator.hpp>

extern "C" char page_dir[];
char *_page_dir = page_dir + KERNEL_PAGE_OFFSET;

void *operator new(size_t, void *address);

namespace memory {

utils::allocator<heap_allocator, 32> *a;

char *allocator_memory = nullptr;

void initialize() {
    utils::fill(_page_dir, 4, 0);
    allocator_memory = sections::__heap_start;
    a = new(allocator_memory) utils::allocator<heap_allocator, 32>(allocator_memory + 64);
}

} // namespacce memory

