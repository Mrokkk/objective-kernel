#pragma once

#define PAGE_SIZE 0x1000
#define PAGE_NUMBER 0x100000
#define PAGE_TABLES_NUMBER 1024
#define PAGE_INIT_FLAGS (0x7)
#define KERNEL_PAGE_OFFSET (0xc0000000)

#ifndef __ASSEMBLER__

namespace memory {

namespace paging {

struct page_directory_entry final {
    uint32_t present : 1;
    uint32_t writeable : 1;
    uint32_t user : 1;
    uint32_t write_through : 1;
    uint32_t cache_disable : 1;
    uint32_t accessed : 1;
    uint32_t reserved : 1;
    uint32_t size : 1;
    uint32_t ignored : 4;
    uint32_t address : 20;
} __packed;

struct page_table_entry final {
    uint32_t present : 1;
    uint32_t writeable : 1;
    uint32_t user : 1;
    uint32_t write_through : 1;
    uint32_t cache_disable : 1;
    uint32_t accessed : 1;
    uint32_t dirty : 1;
    uint32_t reserved : 1;
    uint32_t global : 1;
    uint32_t ignored : 3;
    uint32_t address : 20;
} __packed;

} // namespace paging

extern char *allocator_memory;

void initialize();

} // namespace memory

#endif

