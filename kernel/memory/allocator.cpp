#include <allocator.h>
#include <kernel/memory/heap_allocator.h>
#include <kernel/memory/sections.h>

namespace memory {

static utils::allocator<heap_allocator, 32> a(memory::sections::__heap_start);

} // namespace memory

void *operator new(size_t size) {
    return memory::a.allocate(size);
}

void *operator new(size_t, void *address) {
    return address;
}

void operator delete(void *address) noexcept {
    memory::a.free(address);
}

void operator delete(void *address, size_t) noexcept {
    memory::a.free(address);
}

void *operator new[](size_t size) {
    return memory::a.allocate(size);
}

void operator delete[](void *address) noexcept {
    memory::a.free(address);
}

void operator delete[](void *address, size_t) noexcept {
    memory::a.free(address);
}

