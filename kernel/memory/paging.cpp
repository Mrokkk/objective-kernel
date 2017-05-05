#include <allocator.h>
#include <algorithm.h>
#include <kernel/boot.hpp>
#include <kernel/memory/paging.hpp>
#include <kernel/memory/sections.hpp>
#include <kernel/memory/heap_allocator.hpp>
#include <kernel/console/logger.hpp>

void * operator new(std::size_t, void *address);
void * operator new[](std::size_t, void *address);

asmlinkage memory::paging::page_directory_entry page_dir[];
asmlinkage memory::paging::page_table_entry page0[];

namespace memory {

namespace paging {

page_directory_entry *page_dir = nullptr;
page_table_entry *page_tables = nullptr;

bool frame_is_free(uint32_t addr) {
    uint32_t frame = addr / 4096;
    return !(frames[frame / 32] & (1 << (frame % 32)));
}

inline void page_set(int nr, uint32_t val) {
    reinterpret_cast<uint32_t *>(page_tables)[nr] = val;
}

inline void page_table_set(int nr, uint32_t val) {
    reinterpret_cast<uint32_t *>(page_dir)[nr] = val;
}

inline void frame_alloc(unsigned int i) {
    frames[i / 32] |= (1 << (i % 32));
}

void *page_alloc() {
    uint32_t i;
    uint32_t frame_nr, address;
    for (i = 0; i < 32678 * 32; i++)
        if (frame_is_free(i * PAGE_SIZE)) break;

    frame_alloc(i);
    frame_nr = i;
    address = frame_nr * 4096;
    page_set(i, address | PGT_PRESENT | PGT_WRITEABLE | PGT_USER);
    return (void *)(frame_nr * PAGE_SIZE + KERNEL_PAGE_OFFSET);
}

}

using allocator = utils::allocator<heap_allocator, 32>;

allocator *a = nullptr;
char *allocator_memory = nullptr;
uint32_t frames[32768];

void initialize() {
    unsigned int end = (uint32_t)sections::__heap_start - KERNEL_PAGE_OFFSET,
                 frame_count = end / PAGE_SIZE,
                 count = frame_count /  32,
                 bits = frame_count % 32,
                 i, j;

    paging::page_table_entry *temp_pgt;

    paging::page_dir = virt_address(::page_dir);
    temp_pgt = paging::page_tables = virt_address(::page0);

    for (i = 0; i < count; i++)
        frames[i] = ~0UL;

    frames[count] = (~0UL >> (32 - bits));

    paging::page_tables = static_cast<paging::page_table_entry *>(paging::page_alloc());

    utils::memcopy((const char *)temp_pgt, (char *)paging::page_tables, PAGE_SIZE * 4);

    for (i = 768, j = 0; i < PAGE_TABLES_NUMBER; i++, j+=1024)
        paging::page_table_set(i, phys_address((uint32_t)&paging::page_tables[j]) |
                PGD_PRESENT | PGD_WRITEABLE | PGD_USER);

    for (i = 1; i < 1024; i++)
        paging::page_alloc();

    allocator_memory = (char *)paging::page_alloc();
    auto mem = (char *)paging::page_alloc();
    a = new(allocator_memory) allocator(mem);

    paging::page_set(0, 0);
    paging::page_directory_reload();
    paging::page_alloc();
    paging::page_alloc();
}

} // namespace memory

