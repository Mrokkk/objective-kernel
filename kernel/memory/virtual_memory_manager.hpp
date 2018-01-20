#pragma once

#include <kernel_list.hpp>
#include "memory.hpp"
#include "physical_memory_manager.hpp"

asmlinkage cpu::mmu::page_directory_entry page_dir[];
asmlinkage cpu::mmu::page_table_entry page0[];

namespace memory {

extern cpu::mmu::page_directory_entry kernel_page_dir[];
extern cpu::mmu::page_table_entry kernel_page_table[];

struct address_space {
    cpu::mmu::page_directory_entry *page_dir;
    utils::kernel_list<address_space>::node node;
};

struct virtual_memory_manager {

    explicit virtual_memory_manager(physical_memory_manager *pmm);

    void unmap(cpu::mmu::page_directory_entry *page_dir, void *address);
    cpu::mmu::page_directory_entry *create_address_space();
    void *allocate();

private:
    void initialize();

    inline void page_set(cpu::mmu::page_table_entry *page_table, int nr, uint32_t val) {
        reinterpret_cast<uint32_t *>(page_table)[nr] = val;
    }

    inline void page_clear(cpu::mmu::page_table_entry *page_table, int nr) {
        reinterpret_cast<uint32_t *>(page_table)[nr] = 0;
    }

    inline void page_table_set(cpu::mmu::page_directory_entry *page_dir, int nr, uint32_t val) {
        reinterpret_cast<uint32_t *>(page_dir)[nr] = val;
    }

    inline void page_table_clear(cpu::mmu::page_directory_entry *page_dir, int nr) {
        reinterpret_cast<uint32_t *>(page_dir)[nr] = 0;
    }

    physical_memory_manager *pmm_;
    address_space kernel_address_space_;
    utils::kernel_list<address_space> address_spaces_;
};

} // namespace memory

