#pragma once

#define PAGE_SIZE 0x1000
#define PAGE_NUMBER 0x100000
#define PAGE_TABLES_NUMBER 1024
#define PAGE_INIT_FLAGS (0x7)
#define KERNEL_PAGE_OFFSET (0xc0000000)

#ifndef __ASSEMBLER__

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

namespace memory {

template <typename Type>
inline Type align(Type address, uint32_t alignment) {
    auto mod = (uint32_t)address % alignment;
    if (mod != 0) {
        return reinterpret_cast<Type >((((uint32_t)address + alignment - 1) / 1024) * 1024);
    }
    return address;
}

template <typename Type>
constexpr inline Type virt_address(Type address) {
    return reinterpret_cast<Type>(reinterpret_cast<uint32_t>(address) + KERNEL_PAGE_OFFSET);
}

template <typename Type>
constexpr inline Type phys_address(Type address) {
    return reinterpret_cast<Type>(reinterpret_cast<uint32_t>(address) - KERNEL_PAGE_OFFSET);
}

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

inline void page_directory_load(page_directory_entry *pgd) {
    asm volatile(
            "mov %0, %%cr3;"
            :: "r" (pgd) : "memory");
}

inline void page_directory_reload() {
    int dummy = 0;
    asm volatile(
            "mov %%cr3, %0;"
            "mov %0, %%cr3;"
            : "=r" (dummy)
            : "r" (dummy)
            : "memory"
    );
}

inline void invlpg(void *address) {
    asm volatile(
            "invlpg (%0);"
            :: "r" (address)
            : "memory"
    );
}

bool frame_is_free(uint32_t addr);
void *page_alloc();

class page_allocator final {

    char *heap_;

public:

    explicit page_allocator(char *) {
        heap_ = static_cast<char *>(page_alloc());
        page_alloc(); // FIXME
    }

    void *grow_heap(size_t value) {
        auto prev_heap = heap_;
        auto prev_page = reinterpret_cast<uint32_t>(prev_heap) % PAGE_SIZE;
        heap_ += value;
        auto new_page = reinterpret_cast<uint32_t>(heap_) % PAGE_SIZE;
        if (new_page != prev_page) {
            page_alloc();
        }
        return prev_heap;
    }

};

extern uint32_t frames[];

} // namespace paging

extern char *allocator_memory;

void initialize();

} // namespace memory

#endif

