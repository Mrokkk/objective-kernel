#pragma once

namespace kernel {

namespace memory {

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

} // namespace memory

} // namespace kernel

