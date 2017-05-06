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

uint32_t __end = 0u;

namespace paging {

page_directory_entry *page_dir = nullptr;
page_table_entry *page_tables = nullptr;
uint32_t page_tables_number = 0u;
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

void allocate_frames() {
    // TODO: allocate memory for frames
    frames.set(data, align(boot::upper_mem, 1024) * 1024, __end / PAGE_SIZE);
}

void set_page_directory() {
    for (auto i = 768u, j = 0u; i < PAGE_TABLES_NUMBER; i++, j += 1024) {
        paging::page_table_set(i, phys_address(reinterpret_cast<uint32_t>(&paging::page_tables[j])) |
                PGD_PRESENT | PGD_WRITEABLE | PGD_USER);
    }
}

void initialize() {
    __end = reinterpret_cast<uint32_t>(phys_address(sections::__heap_start));
    paging::page_dir = virt_address(::page_dir);
    paging::page_table_entry *temp_pgt = paging::page_tables = virt_address(::page0);
    paging::allocate_frames();
    paging::page_tables = static_cast<paging::page_table_entry *>(paging::page_alloc());
    utils::copy(temp_pgt, paging::page_tables, PAGE_SIZE);
    paging::set_page_directory();
    page_tables_number = align(boot::upper_mem, 1024) / PAGE_SIZE;
    for (auto i = 1u; i < page_tables_number; i++) {
        paging::page_alloc();
    }
    paging::page_table_set(0, 0);
    paging::page_directory_reload();
}

}

} // namespace memory

