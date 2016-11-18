#include <kernel/allocator.h>
#include <kernel/heap_allocator.hpp>
#include <basic/linkage.h>
#include <stddef.h>

asmlinkage char _end[];

namespace kernel {
static allocator<heap_allocator, 16> a(_end);
} // namespace kernel

void *operator new(size_t size) {
    return kernel::a.allocate(size);
}

void *operator new(size_t, void *address) {
    return address;
}

void operator delete(void *address) noexcept {
    kernel::a.free(address);
}

void operator delete(void *address, size_t) noexcept {
    kernel::a.free(address);
}

void *operator new[](size_t size) {
    return kernel::a.allocate(size);
}

void operator delete[](void *address) noexcept {
    kernel::a.free(address);
}

void operator delete[](void *address, size_t) noexcept {
    kernel::a.free(address);
}

