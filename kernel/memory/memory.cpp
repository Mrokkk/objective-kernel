#include <allocator.hpp>

#include <kernel/logger/logger.hpp>

#include "memory.hpp"
#include "paging/paging.hpp"

namespace memory {

using allocator = utils::allocator<paging::page_allocator, 32>;

allocator kernel_allocator(nullptr);

void initialize() {
    paging::initialize();
}

} // namespace memory

void *operator new(size_t size) {
    return memory::kernel_allocator.allocate(size);
}

void operator delete(void *address) noexcept {
    memory::kernel_allocator.free(address);
}

void operator delete(void *address, size_t) noexcept {
    memory::kernel_allocator.free(address);
}

void *operator new[](size_t size) {
    return memory::kernel_allocator.allocate(size);
}

void operator delete[](void *address) noexcept {
    memory::kernel_allocator.free(address);
}

void operator delete[](void *address, size_t) noexcept {
    memory::kernel_allocator.free(address);
}

