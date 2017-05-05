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
}

} // namespacce memory

