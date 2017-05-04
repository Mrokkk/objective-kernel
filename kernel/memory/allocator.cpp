#include <allocator.h>
#include <kernel/memory/sections.hpp>
#include <kernel/memory/paging.hpp>
#include <kernel/memory/heap_allocator.hpp>

namespace memory {

extern utils::allocator<heap_allocator, 32> *a;

} // namespace memory

void *operator new(size_t size) {
    return memory::a->allocate(size);
}

void *operator new(size_t, void *address) {
    return address;
}

void operator delete(void *address) noexcept {
    memory::a->free(address);
}

void operator delete(void *address, size_t) noexcept {
    memory::a->free(address);
}

void *operator new[](size_t size) {
    return memory::a->allocate(size);
}

void operator delete[](void *address) noexcept {
    memory::a->free(address);
}

void operator delete[](void *address, size_t) noexcept {
    memory::a->free(address);
}

