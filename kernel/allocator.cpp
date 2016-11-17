#include <kernel/allocator.h>
#include <basic/linkage.h>
#include <cstdint>

asmlinkage char _end[];

namespace kernel {
static allocator<16> a(_end);
}

void *operator new(unsigned int size) {
    return kernel::a.allocate(size);
}

void *operator new(unsigned int, void *address) {
    return address;
}

void operator delete(void *address) noexcept {
    kernel::a.free(address);
}

void operator delete(void *address, unsigned int) noexcept {
    kernel::a.free(address);
}

void *operator new[](unsigned int size) {
    return kernel::a.allocate(size);
}

void operator delete[](void *address) noexcept {
    kernel::a.free(address);
}

void operator delete[](void *address, unsigned int) noexcept {
    kernel::a.free(address);
}

