#include <kernel/allocator.h>
#include <kernel/heap_allocator.h>
#include <arch/sections.h>

namespace kernel {

static allocator<heap_allocator, 32> a(memory::sections::__heap_start);

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

