#include <allocator.h>
#include <algorithm.h>
#include <kernel/boot.hpp>
#include <kernel/memory/sections.hpp>
#include <kernel/console/logger.hpp>

#include "paging.hpp"
#include "frames_allocator.hpp"

asmlinkage memory::paging::page_directory_entry page_dir[];
asmlinkage memory::paging::page_table_entry page0[];

namespace memory {

namespace paging {

page_directory_entry *page_dir = nullptr;
page_table_entry *page_tables = nullptr;
uint32_t data[32768];
frames_allocator frames;

inline void page_set(int nr, uint32_t val) {
    reinterpret_cast<uint32_t *>(page_tables)[nr] = val;
}

inline void page_table_set(int nr, uint32_t val) {
    reinterpret_cast<uint32_t *>(page_dir)[nr] = val;
}

void *page_alloc() {
    uint32_t i;
    auto address = frames.allocate();
    i = address / PAGE_SIZE;
    page_set(i, address | PGT_PRESENT | PGT_WRITEABLE | PGT_USER);
    return reinterpret_cast<void *>(virt_address(address));
}

void allocate_frames(size_t n) {
    frames.set(data, n);
}

void set_page_directory() {
    for (auto i = 768u, j = 0u; i < PAGE_TABLES_NUMBER; i++, j += 1024) {
        paging::page_table_set(i, phys_address(reinterpret_cast<uint32_t>(&paging::page_tables[j])) |
                PGD_PRESENT | PGD_WRITEABLE | PGD_USER);
    }
}

void initialize() {
    // TODO: create only page tables for available RAM
    unsigned int end = reinterpret_cast<uint32_t>(phys_address(sections::__heap_start)),
                 frame_count = end / PAGE_SIZE;
    paging::page_dir = virt_address(::page_dir);
    paging::page_table_entry *temp_pgt = paging::page_tables = virt_address(::page0);
    paging::allocate_frames(frame_count);
    paging::page_tables = static_cast<paging::page_table_entry *>(paging::page_alloc());
    utils::copy(temp_pgt, paging::page_tables, PAGE_SIZE);
    paging::set_page_directory();
    for (auto i = 1u; i < 1024; i++) {
        paging::page_alloc();
    }
    paging::page_table_set(0, 0);
    paging::page_directory_reload();
}

}

} // namespace memory

