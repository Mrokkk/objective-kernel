#include <allocator.h>
#include <kernel/memory/paging/paging.hpp>
#include <kernel/console/console.hpp>

asmlinkage {

extern void (*__init_array_start)();
extern void (*__init_array_end)();
extern void (*__preinit_array_start)();
extern void (*__preinit_array_end)();

void * __dso_handle = 0;

int __cxa_atexit(void (*)(void *), void *, void *) {
    return 0;
}

void __cxa_pure_virtual() {
    console::cout << "PANIC: called pure virtual method\n";
    while (1);
}

// for clang
void *memset(void *ptr, int value, unsigned n) {
    for (auto i = 0u; i < n; ++i) {
        *reinterpret_cast<char *>(ptr) = value;
    }
    return ptr;
}

}

namespace memory {

extern utils::allocator<memory::paging::page_allocator, 32> kernel_allocator;

} // namespace memory

void *operator new(size_t size) {
    return memory::kernel_allocator.allocate(size);
}

void *operator new(size_t, void *address) {
    return address;
}

void * operator new[] (std::size_t, void *address) {
    return address;
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

namespace cpp_support {

void initialize() {
    void (**preinit_constructor)() = &__preinit_array_start;
    void (**init_constructor)() = &__init_array_start;
    while (preinit_constructor != &__preinit_array_end) {
        (*preinit_constructor)();
        ++preinit_constructor;
    }
    while (init_constructor != &__init_array_end) {
        (*init_constructor)();
        ++init_constructor;
    }
}

} // namespace cpp_support

