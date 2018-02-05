#include <kernel/memory/memory.hpp>

#include "../boot.hpp"
#include "../multiboot.hpp"
#include "common.hpp"

namespace boot {
namespace detail {

asmlinkage SECTION(.boot)
void read_bootloader_data(void *tag) {
    auto module_index = 0u;
    auto boot_data = boot_data_physical();
    for (auto temp = reinterpret_cast<mb2_tag *>(static_cast<mb2_tags_header *>(tag) + 1); temp->type != 0;
            temp = reinterpret_cast<mb2_tag *>(((char *)temp + ((temp->size + 7) & ~7)))) {
        switch (temp->type) {
            case MULTIBOOT_TAG_TYPE_CMDLINE:
                strcpy(reinterpret_cast<char *>(temp + 1), boot_data->cmdline);
                break;
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                strcpy(reinterpret_cast<char *>(temp + 1), boot_data->bootloader_name);
                break;
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                boot_data->upper_mem = ((uint32_t *)temp)[3];
                break;
            case MULTIBOOT_TAG_TYPE_MODULE:
            {
                auto module_tag = reinterpret_cast<mb2_module_tag *>(temp);
                auto modules_array = reinterpret_cast<module *>(reinterpret_cast<uint32_t>(::boot::modules) - KERNEL_PAGE_OFFSET);
                modules_array[module_index].start = module_tag->mod_start;
                modules_array[module_index].end = module_tag->mod_end;
                strcpy(module_tag->name, modules_array[module_index].name);
                ++module_index;
                break;
            }
            default:
                break;
        }
    }
}

} // namespace detail
} // namespace boot

