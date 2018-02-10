#include <lazy_instance.hpp>
#include <kernel/boot/boot.hpp>
#include <kernel/logger/logger.hpp>
#include <kernel/memory/sections.hpp>

#include "allocator.hpp"
#include "memory.hpp"
#include "physical_memory_manager.hpp"
#include "virtual_memory_manager.hpp"

namespace memory {

utils::lazy_instance<physical_memory_manager> pmm;
utils::lazy_instance<virtual_memory_manager> vmm;
utils::lazy_instance<allocator<virtual_memory_manager, memory_block_size>> heap_allocator;
struct mmap memory_map[16];

void initialize() {
    pmm.initialize(align(boot::boot_data.upper_mem, 1024) * 1024,
        reinterpret_cast<uint32_t>(virt2phys(sections::__heap_start)));
    vmm.initialize(pmm.get());
    heap_allocator.initialize(vmm.get());
    for (auto i = 0u; i < boot::boot_data.memory_map_size; ++i) {
        logger("memory") << logger::info << "mmap[" << (int)i << "]: " << memory::memory_map[i].base
            << " size: " << memory::memory_map[i].size
            << " type: " << (int)memory::memory_map[i].type;
    }
}

} // namespace memory

