#include <allocator.h>
#include <algorithm.h>
#include <kernel/boot.hpp>
#include <kernel/memory/paging.hpp>
#include <kernel/memory/sections.hpp>
#include <kernel/memory/heap_allocator.hpp>

extern "C" char page_dir[];

void *operator new(size_t, void *address);

namespace memory {

using allocator = utils::allocator<heap_allocator, 32>;

char *_page_dir = virt_address(page_dir);
allocator *a = nullptr;
char *allocator_memory = nullptr;
uint32_t frames_size = 0u;
uint32_t *frames = nullptr;

void initialize() {
    utils::fill(_page_dir, 4, 0);
    allocator_memory = sections::__heap_start;
    frames_size = 32768 * (boot::upper_mem / 1024) / 4096; // FIXME: align to MiB
    frames = new(align(allocator_memory, 4096)) uint32_t;
    allocator_memory = align(allocator_memory + frames_size, 4096);
    a = new(allocator_memory) allocator(align(allocator_memory + sizeof(allocator), 4096));
}

} // namespacce memory

