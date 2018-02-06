#include "memory.hpp"
#include "virtual_memory_manager.hpp"
#include "physical_memory_manager.hpp"

using namespace cpu::mmu;

asmlinkage page_directory_entry page_dir[];
asmlinkage page_table_entry page0[];

namespace memory {

page_directory_entry kernel_page_dir[1024] ALIGN(PAGE_SIZE);
page_table_entry kernel_page_table[1024] ALIGN(PAGE_SIZE);

virtual_memory_manager::virtual_memory_manager(physical_memory_manager* pmm)
        : pmm_(pmm)
        , address_spaces_(&address_space::node)
        , log_("virtual_memory_manager") {
    initialize();
}

void virtual_memory_manager::unmap(page_directory_entry* page_dir, void* address) {
    const auto table_index = reinterpret_cast<uint32_t>(address) >> 22;
    const auto page_index = (reinterpret_cast<uint32_t>(address) >> 12) & 0x03FF;
    auto page_table = phys2virt(static_cast<uint32_t>(page_dir[table_index].address) << 12);
    page_clear(reinterpret_cast<page_table_entry *>(page_table), page_index);
    invlpg(address);
}

page_directory_entry *virtual_memory_manager::create_address_space() {
    auto page_dir = allocate();
    utils::copy((uint32_t*)kernel_page_dir, (uint32_t*)page_dir, 1024);
    return static_cast<page_directory_entry*>(page_dir);
}

void *virtual_memory_manager::allocate() {
    auto frame = pmm_->allocate_frame();
    page_set(kernel_page_table, frame.index, frame.address | PGT_PRESENT | PGT_WRITEABLE);
    return reinterpret_cast<void*>(phys2virt(frame.address));
}

void virtual_memory_manager::initialize() {
    clear_dir_or_table(kernel_page_dir);
    clear_dir_or_table(kernel_page_table);
    const auto heap_start = pmm_->end();
    const auto needed_pages = heap_start / PAGE_SIZE;
    for (auto i = 0u; i < needed_pages; ++i) {
        page_set(kernel_page_table, i, (i * PAGE_SIZE) | PGT_PRESENT | PGT_WRITEABLE);
    }
    page_table_set(kernel_page_dir, page_table_index(KERNEL_PAGE_OFFSET),
            virt2phys(reinterpret_cast<uint32_t>(kernel_page_table)) |
                PGD_PRESENT | PGD_WRITEABLE);
    kernel_address_space_.page_dir = kernel_page_dir;
    address_spaces_.push_back(kernel_address_space_);
    page_table_clear(kernel_page_dir, 0);
    page_directory_load(virt2phys(kernel_page_dir));
    unmap(kernel_page_dir, phys2virt(page_dir));

    pmm_->deallocate_frame(reinterpret_cast<uint32_t>(page_dir));
    unmap(kernel_page_dir, phys2virt(page0));
    pmm_->deallocate_frame(reinterpret_cast<uint32_t>(page0));
}

} // namespace memory

