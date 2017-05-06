#include <string.h>

#include <kernel/memory/paging/paging.hpp>

#include "boot.hpp"
#include "multiboot.hpp"

namespace boot {

char cmdline[128];
uint32_t upper_mem = 0u;

__section(.text.boot)
char *get_mb2_cmdline(mb2_tags_header *tag) {
    for (auto temp = reinterpret_cast<mb2_tag *>(tag + 1); temp->type != 0;
            temp = reinterpret_cast<mb2_tag *>(((char *)temp + ((temp->size + 7) & ~7)))) {
        if (temp->type == 1) {
            return reinterpret_cast<char *>(++temp);
        }
    }
    return nullptr;
}

__section(.text.boot)
uint32_t get_mb2_upper_mem(mb2_tags_header *tag) {
    for (auto temp = reinterpret_cast<mb2_tag *>(tag + 1); temp->type != 0;
            temp = reinterpret_cast<mb2_tag *>(((uint8_t *)temp + ((temp->size + 7) & ~7)))) {
        if (temp->type == 4) {
            return ((uint32_t *)temp)[3];
        }
    }
    return 0;
}

__section(.text.boot)
uint32_t read_upper_mem(void *data, uint32_t magic) {
    if (magic == MULTIBOOT_BOOTLOADER_MAGIC)
        return static_cast<multiboot_info *>(data)->mem_upper;
    else if (magic == MULTIBOOT2_BOOTLOADER_MAGIC)
        return get_mb2_upper_mem(static_cast<mb2_tags_header *>(data));
    return 0xffffffff;
}

__section(.text.boot)
char *read_cmdline(void *data, uint32_t magic) {
    if (magic == MULTIBOOT_BOOTLOADER_MAGIC)
        return static_cast<multiboot_info *>(data)->cmdline;
    else if (magic == MULTIBOOT2_BOOTLOADER_MAGIC)
        return get_mb2_cmdline(static_cast<mb2_tags_header *>(data));
    return nullptr;
}

asmlinkage __section(.text.boot)
void read_bootloader_data(void *data, uint32_t magic) {
    auto cmdline_ptr = read_cmdline(data, magic);
    if (cmdline_ptr == nullptr) {
        cmdline[0] = 0;
    }
    else {
        auto ptr = reinterpret_cast<char *>(reinterpret_cast<uint32_t>(cmdline - KERNEL_PAGE_OFFSET));
        while (*cmdline_ptr) {
            *ptr++ = *cmdline_ptr++;
        }
        *ptr = 0;
    }
    *reinterpret_cast<uint32_t *>(reinterpret_cast<uint32_t>(&upper_mem) - KERNEL_PAGE_OFFSET)
        = read_upper_mem(data, magic);
}

} // namespace bootloader

