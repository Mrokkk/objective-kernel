#include <string.h>

#include "boot.hpp"
#include "multiboot.hpp"

namespace boot {

char cmdline[128];

char *get_mb2_cmdline(mb2_tags_header *tag) {
    auto temp = reinterpret_cast<mb2_tag *>(++tag);
    for (auto i = 0; temp->type != 0; ++i) {
        if (temp->type == 1) {
            return (char *)++temp;
        }
        temp = reinterpret_cast<mb2_tag *>((char *)tag + ((temp->size + 7) & ~7));
    }
    return nullptr;
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
}


} // namespace bootloader
