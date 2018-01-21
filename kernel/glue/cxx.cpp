#include <kernel/logger/logger.hpp>
#include <kernel/memory/allocator.hpp>

void *operator new(size_t size) {
    return memory::heap_allocator->allocate(size);
}

void operator delete(void *address) noexcept {
    memory::heap_allocator->free(address);
}

void operator delete(void *address, size_t) noexcept {
    memory::heap_allocator->free(address);
}

void *operator new[](size_t size) {
    return memory::heap_allocator->allocate(size);
}

void operator delete[](void *address) noexcept {
    memory::heap_allocator->free(address);
}

void operator delete[](void *address, size_t) noexcept {
    memory::heap_allocator->free(address);
}

asmlinkage {

void *__dso_handle = nullptr;

int __cxa_atexit(void (*)(void *), void *, void *) {
    return 0;
}

void __cxa_pure_virtual() {
    logger("cxx") << logger::log_level::error << "PANIC: called pure virtual method\n";
    while (1) {
        cpu::halt();
    }
}

// for clang
void *memset(void *ptr, int value, unsigned n) {
    for (auto i = 0u; i < n; ++i) {
        *reinterpret_cast<char *>(ptr) = value;
    }
    return ptr;
}

using init_fn = void (*)();

asmlinkage init_fn __init_array_start[];
asmlinkage init_fn __init_array_end[];

asmlinkage init_fn __preinit_array_start[];
asmlinkage init_fn __preinit_array_end[];

void global_constructors() {
    for (auto init_constructor = __preinit_array_start; init_constructor != __preinit_array_end; ++init_constructor) {
        (*init_constructor)();
    }
    for (auto init_constructor = __init_array_start; init_constructor != __init_array_end; ++init_constructor) {
        (*init_constructor)();
    }
}

} // asmlinkage

