#include <string.h>

#include <kernel/memory/paging.hpp>

#include "boot.hpp"
#include "multiboot.hpp"

namespace boot {

char cmdline[128];
uint32_t upper_mem = 0u;

char *get_mb2_cmdline(mb2_tags_header *tag) {
    for (auto temp = reinterpret_cast<mb2_tag *>(memory::virt_address(tag) + 1); temp->type != 0;
            temp = reinterpret_cast<mb2_tag *>(((char *)temp + ((temp->size + 7) & ~7)))) {
        if (temp->type == 1) {
            return (char *)++temp;
        }
    }
    return nullptr;
}


uint32_t get_mb2_upper_mem(mb2_tags_header *tag) {
    for (auto temp = reinterpret_cast<mb2_tag *>(memory::virt_address(tag) + 1); temp->type != 0;
            temp = reinterpret_cast<mb2_tag *>(((uint8_t *)temp + ((temp->size + 7) & ~7)))) {
        if (temp->type == 4) {
            return ((uint32_t *)temp)[3];
        }
    }
    return 0;
}

uint32_t read_upper_mem(void *data, uint32_t magic) {
    if (magic == MULTIBOOT_BOOTLOADER_MAGIC)
        return static_cast<multiboot_info *>(data)->mem_upper;
    else if (magic == MULTIBOOT2_BOOTLOADER_MAGIC)
        return get_mb2_upper_mem(reinterpret_cast<mb2_tags_header *>(data));
    return 0;
}

char *read_cmdline(void *data, uint32_t magic) {
    if (magic == MULTIBOOT_BOOTLOADER_MAGIC)
        return static_cast<multiboot_info *>(data)->cmdline;
    else if (magic == MULTIBOOT2_BOOTLOADER_MAGIC)
        return get_mb2_cmdline(reinterpret_cast<mb2_tags_header *>(data));
    return nullptr;
}

asmlinkage void read_bootloader_data(void *data, uint32_t magic) {
    auto cmdline_ptr = read_cmdline(data, magic);
    if (cmdline_ptr == nullptr) {
        cmdline[0] = 0;
    }
    else {
        utils::copy(cmdline_ptr, cmdline);
    }
    upper_mem = read_upper_mem(data, magic);
}

} // namespace bootloader

