#include <algorithm.hpp>
#include <drivers/serial.hpp>
#include <kernel/boot/boot.hpp>
#include <kernel/console/console.hpp>
#include <kernel/logger/logger.hpp>
#include <kernel/memory/sections.hpp>

#include "allocator.hpp"
#include "memory.hpp"
#include "physical_memory_manager.hpp"
#include "virtual_memory_manager.hpp"

asmlinkage cpu::mmu::page_directory_entry page_dir[];
asmlinkage cpu::mmu::page_table_entry page0[];

namespace memory {

cpu::mmu::page_directory_entry kernel_page_dir[1024] __attribute__((aligned(4096)));
cpu::mmu::page_table_entry kernel_page_table[1024] __attribute__((aligned(4096)));

char pmm_memory[sizeof(physical_memory_manager)];
physical_memory_manager *pmm;

char vmm_memory[sizeof(physical_memory_manager)];
virtual_memory_manager *vmm;

char allocator_memory[sizeof(allocator<virtual_memory_manager, 32>)];
allocator<virtual_memory_manager, 32> *kernel_allocator = nullptr;

void initialize() {
    pmm = new(pmm_memory) physical_memory_manager(align(boot::upper_mem, 1024) * 1024, reinterpret_cast<uint32_t>(phys_address(sections::__heap_start)));
    vmm = new(vmm_memory) virtual_memory_manager(pmm);
    kernel_allocator = new(allocator_memory) allocator<virtual_memory_manager, 32>(vmm);
}

} // namespace memory

// TODO: move to cxx.cpp
void *operator new(size_t size) {
    return memory::kernel_allocator->allocate(size);
}

void operator delete(void *address) noexcept {
    memory::kernel_allocator->free(address);
}

void operator delete(void *address, size_t) noexcept {
    memory::kernel_allocator->free(address);
}

void *operator new[](size_t size) {
    return memory::kernel_allocator->allocate(size);
}

void operator delete[](void *address) noexcept {
    memory::kernel_allocator->free(address);
}

void operator delete[](void *address, size_t) noexcept {
    memory::kernel_allocator->free(address);
}

