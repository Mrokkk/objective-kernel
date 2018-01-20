#include <kernel/boot/boot.hpp>
#include <kernel/lazy_instance.hpp>
#include <kernel/memory/sections.hpp>

#include "allocator.hpp"
#include "memory.hpp"
#include "physical_memory_manager.hpp"
#include "virtual_memory_manager.hpp"

namespace memory {

utils::lazy_instance<physical_memory_manager> pmm;
utils::lazy_instance<virtual_memory_manager> vmm;
utils::lazy_instance<allocator<virtual_memory_manager, memory_block_size>> heap_allocator;

void initialize() {
    pmm.initialize(align(boot::upper_mem, 1024) * 1024,
        reinterpret_cast<uint32_t>(virt2phys(sections::__heap_start)));
    vmm.initialize(pmm.get());
    heap_allocator.initialize(vmm.get());
}

} // namespace memory

