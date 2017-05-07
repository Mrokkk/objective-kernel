#include <allocator.h>
#include <kernel/console/logger.hpp>

#include "paging/paging.hpp"

namespace memory {

using allocator = utils::allocator<paging::page_allocator, 32>;

allocator *kernel_allocator = nullptr;
char *allocator_memory = nullptr;

void initialize() {
    paging::initialize();
    allocator_memory = (char *)paging::page_alloc();
    kernel_allocator = new(allocator_memory) allocator(0);
}

} // namespace memory

