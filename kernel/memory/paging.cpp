#include <allocator.h>
#include <algorithm.h>
#include <kernel/boot.hpp>
#include <kernel/memory/paging.hpp>
#include <kernel/memory/sections.hpp>
#include <kernel/memory/heap_allocator.hpp>
#include <kernel/console/logger.hpp>

void * operator new(std::size_t, void *address);
void * operator new[](std::size_t, void *address);

asmlinkage memory::paging::page_directory_entry *page_dir;
asmlinkage memory::paging::page_table_entry *page0;

namespace memory {

using allocator = utils::allocator<heap_allocator, 32>;

paging::page_directory_entry *page_dir = virt_address(::page_dir);
paging::page_table_entry *page_tables = nullptr;
allocator *a = nullptr;
char *allocator_memory = nullptr;
uint32_t frames_size = 0u;
uint32_t *frames = nullptr;

void allocate_frames() {
    frames_size = align(32768 * (boot::upper_mem / 1024) / 4096, 1024);
    frames = new(align(allocator_memory, 8)) uint32_t[frames_size];
    utils::fill(frames, frames_size, 0);
    allocator_memory = align(allocator_memory + frames_size, 0x1000);
}

void allocate_page_tables() {
    page_tables = new(allocator_memory) paging::page_table_entry[frames_size * 32];
    allocator_memory = align(allocator_memory + frames_size * 32 * sizeof(paging::page_table_entry), 0x1000);
}

void create_allocator() {
    a = new(allocator_memory) allocator(align(allocator_memory + sizeof(allocator), 0x1000));
    allocator_memory = align(allocator_memory + 0xfff, 0x1000);
}

namespace paging {

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
    uint32_t i, end = (uint32_t)::memory::allocator_memory - KERNEL_PAGE_OFFSET;
    uint32_t frame_nr, address;
    for (i = end / PAGE_SIZE; i < ::boot::upper_mem * 1024 / PAGE_SIZE; i++)
        if (frame_is_free(i * PAGE_SIZE)) break;
    frame_alloc(i);
    frame_nr = i;
    address = frame_nr * 4096;
    page_set(i, address | PGT_PRESENT | PGT_WRITEABLE | PGT_USER);
    assert(address >= end);
    assert(page_tables[address / PAGE_SIZE].address != 0);
    return (void *)virt_address(frame_nr * PAGE_SIZE);
}

}

void initialize() {
    utils::fill(reinterpret_cast<uint8_t *>(page_dir), 4, 0);
    allocator_memory = sections::__heap_start;
    allocate_frames();
    allocate_page_tables();
    create_allocator();
    uint32_t end = reinterpret_cast<uint32_t>(phys_address(allocator_memory)),
        frame_count = end / PAGE_SIZE + 1,
        count = frame_count / 32,
        bits = frame_count % 32;
    for (auto i = 0u; i < count; i++)
        frames[i] = ~0UL;
    frames[count] = (~0UL >> (32 - bits));
    for (uint32_t i = 0u, value = PAGE_INIT_FLAGS; value < reinterpret_cast<uint32_t>(align(phys_address(allocator_memory), 0x1000) + PAGE_INIT_FLAGS); value += 0x1000, ++i) {
        paging::page_set(i, value);
    }
    //uint32_t i, j;
    //for (i = 768, j = 0; j < (uint32_t)phys_address(allocator_memory) / 0x1000 / 1024; i++, j+=1024)
        //paging::page_table_set(i, 0);
    //}
    //paging::page_directory_load(::page_dir);
}

} // namespace memory

