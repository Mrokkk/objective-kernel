#include <kernel/memory/paging/paging.hpp>

#include "../boot.hpp"
#include "../multiboot.hpp"

namespace boot {

namespace detail {

__section(.text.boot)
void strcpy(const char *from, char *to) {
    while (*from) {
        *to++ = *from++;
    }
    *to = 0;
}

__section(.text.boot)
uint32_t *phys_address(uint32_t *a) {
    return reinterpret_cast<uint32_t *>(reinterpret_cast<uint32_t>(a) - KERNEL_PAGE_OFFSET);
}

__section(.text.boot)
char *phys_address(char *a) {
    return reinterpret_cast<char *>(reinterpret_cast<uint32_t>(a) - KERNEL_PAGE_OFFSET);
}

__section(.text.boot)
void read_bootloader(void *tag) {
    for (auto temp = reinterpret_cast<mb2_tag *>(static_cast<mb2_tags_header *>(tag) + 1); temp->type != 0;
            temp = reinterpret_cast<mb2_tag *>(((char *)temp + ((temp->size + 7) & ~7)))) {
        switch (temp->type) {
            case MULTIBOOT_TAG_TYPE_CMDLINE:
                strcpy(reinterpret_cast<char *>(temp + 1), phys_address(::boot::cmdline));
                break;
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                strcpy(reinterpret_cast<char *>(temp + 1), phys_address(::boot::bootloader_name));
                break;
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                *phys_address(&::boot::upper_mem) = ((uint32_t *)temp)[3];
                break;
            default:
                break;
        }
    }
}

} // namespace detail

} // namespace boot

