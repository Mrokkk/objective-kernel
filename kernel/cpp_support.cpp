#include <allocator.hpp>
#include <kernel/memory/paging/paging.hpp>
#include <kernel/console/console.hpp>

asmlinkage {

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

} // asmlinkage

namespace memory {

extern utils::allocator<memory::paging::page_allocator, 32> kernel_allocator;

} // namespace memory

void *operator new(size_t size) {
    return memory::kernel_allocator.allocate(size);
}

void *operator new(size_t, void *address) {
    return address;
}

void * operator new[](std::size_t, void *address) {
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

void atomic_increment(void *addr) {
    asm volatile("lock incl (%0)" :: "r" (addr));
}

void atomic_decrement(void *addr) {
    asm volatile("lock decl (%0)" :: "r" (addr));
}

void spinlock_lock(volatile size_t *lock) {
    size_t dummy = SPINLOCK_LOCKED;
    asm volatile(R"(
        1: lock xchg %0, %1
        test %1, %1
        jnz 1b)"
        : "=m" (*lock)
        : "r" (dummy)
        : "memory");
}

void spinlock_unlock(volatile size_t *lock) {
    size_t dummy = SPINLOCK_UNLOCKED;
    asm volatile(
        "lock xchg %0, %1"
        : "=m" (*lock)
        : "r" (dummy)
        : "memory");
}

namespace cpp_support {

using init_fn = void (*)();

asmlinkage init_fn __init_array_start[];
asmlinkage init_fn __init_array_end[];

asmlinkage init_fn __preinit_array_start[];
asmlinkage init_fn __preinit_array_end[];

void initialize() {
    for (auto init_constructor = __preinit_array_start; init_constructor != __preinit_array_end; ++init_constructor) {
        (*init_constructor)();
    }
    for (auto init_constructor = __init_array_start; init_constructor != __init_array_end; ++init_constructor) {
        (*init_constructor)();
    }
}

} // namespace cpp_support

