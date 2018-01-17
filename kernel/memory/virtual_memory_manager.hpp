#pragma once

#include <kernel_list.hpp>
#include <iterator.hpp>
#include <algorithm.hpp>
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

    explicit virtual_memory_manager(physical_memory_manager *pmm)
            : pmm_(pmm)
            , address_spaces_(&address_space::node) {
        initialize();
    }

    void initialize() {
        utils::fill((uint32_t *)kernel_page_dir, (uint32_t *)kernel_page_dir + 1024, 0);
        utils::fill((uint32_t *)kernel_page_table, (uint32_t *)kernel_page_table + 1024, 0);
        const auto heap_start = pmm_->end();
        const auto needed_pages = heap_start / 4096;
        for (auto i = 0u; i < needed_pages; ++i) {
            page_set(kernel_page_table, i, (i * PAGE_SIZE) | PGT_PRESENT | PGT_WRITEABLE);
        }
        page_table_set(kernel_page_dir, page_table_index(KERNEL_PAGE_OFFSET),
                phys_address(reinterpret_cast<uint32_t>(kernel_page_table)) |
                    PGD_PRESENT | PGD_WRITEABLE);
        kernel_address_space_.page_dir = kernel_page_dir;
        address_spaces_.push_back(kernel_address_space_);
        page_table_clear(kernel_page_dir, 0);
        cpu::mmu::page_directory_load(phys_address(kernel_page_dir));
        unmap(kernel_page_dir, virt_address(page_dir));
        pmm_->deallocate(reinterpret_cast<uint32_t>(page_dir));
        unmap(kernel_page_dir, virt_address(page0));
        pmm_->deallocate(reinterpret_cast<uint32_t>(page0));
    }

    void unmap(cpu::mmu::page_directory_entry *page_dir, void *address) {
        const auto table_index = page_table_index((uint32_t)address);
        const auto page_index = frame_number((uint32_t)address - table_index * 4096 * 1024);
        auto page_table = virt_address(((cpu::mmu::page_table_entry *)(page_dir[table_index].address << 12)));
        page_clear(page_table, page_index);
        cpu::mmu::invlpg(address);
    }

    cpu::mmu::page_directory_entry *create_address_space() {
        auto page_dir = static_cast<cpu::mmu::page_directory_entry *>(allocate());
        utils::copy((uint32_t *)kernel_page_dir, (uint32_t *)page_dir, 1024);
        return page_dir;
    }

    void *allocate() {
        auto address = pmm_->allocate();
        auto frame_index = frame_number(address);
        page_set(kernel_page_table, frame_index, address | PGT_PRESENT | PGT_WRITEABLE | PGT_USER);
        return reinterpret_cast<void *>(virt_address(address));
    }

private:
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

