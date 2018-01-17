#pragma once

#define PGD_PRESENT     (1 << 0)
#define PGD_WRITEABLE   (1 << 1)
#define PGD_USER        (1 << 2)
#define PGD_WRITETHR    (1 << 3)
#define PGD_CACHEDIS    (1 << 4)
#define PGD_ACCESSED    (1 << 5)
#define PGD_RESERVED    (1 << 6)
#define PGD_SIZE        (1 << 7)

#define PGT_PRESENT     (1 << 0)
#define PGT_WRITEABLE   (1 << 1)
#define PGT_USER        (1 << 2)
#define PGT_WRITETHR    (1 << 3)
#define PGT_CACHEDIS    (1 << 4)
#define PGT_ACCESSED    (1 << 5)
#define PGT_DIRTY       (1 << 6)
#define PGT_RESERVED    (1 << 7)
#define PGT_GLOBAL      (1 << 8)

#define PAGE_SIZE 0x1000
#define PAGE_NUMBER 0x100000
#define PAGE_TABLES_NUMBER 1024
#define PAGE_FLAGS_FOR_1MB (PGT_PRESENT | PGT_WRITEABLE)
#define PAGE_INIT_FLAGS (PGT_PRESENT | PGT_WRITEABLE | PGT_USER)
#define KERNEL_PAGE_OFFSET (0xc0000000)

#ifndef __ASSEMBLER__

#include "registers.hpp"

namespace cpu {

namespace mmu {

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
} PACKED;

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
} PACKED;

inline void page_directory_load(page_directory_entry *pgd) {
    registers::cr3_set(reinterpret_cast<uint32_t>(pgd));
}

inline void page_directory_reload() {
    registers::cr3_set(cpu::registers::cr3_get());
}

inline void invlpg(void *address) {
    asm volatile(
            "invlpg (%0);"
            :: "r" (address)
            : "memory"
    );
}

} // namespace mmu

} // namespace cpu

#endif

