#include <kernel/memory/memory.hpp>

#include "../boot.hpp"
#include "../multiboot.hpp"
#include "common.hpp"

namespace boot {
namespace detail {

SECTION(.boot)
enum memory::mmap::type convert_memory_type(const uint32_t type) {
    switch (type) {
        case 1:
            return memory::mmap::type::available;
        case 2:
            return memory::mmap::type::unavailable;
        case 3:
            return memory::mmap::type::device;
        case 4:
            return memory::mmap::type::device;
        default:
            return memory::mmap::type::unknown;
    }
}

asmlinkage SECTION(.boot)
void read_bootloader_data(void* tag) {
    auto module_index = 0u;
    auto boot_data = boot_data_physical();
    for (auto temp = reinterpret_cast<mb2_tag*>(static_cast<mb2_tags_header*>(tag) + 1); temp->type != 0;
            temp = reinterpret_cast<mb2_tag*>(((char*)temp + ((temp->size + 7) & ~7)))) {
        switch (temp->type) {
            case MULTIBOOT_TAG_TYPE_CMDLINE:
                strcpy(reinterpret_cast<char*>(temp + 1), boot_data->cmdline);
                break;
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                strcpy(reinterpret_cast<char*>(temp + 1), boot_data->bootloader_name);
                break;
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                boot_data->upper_mem = ((uint32_t*)temp)[3];
                boot_data->lower_mem = ((uint32_t*)temp)[2];
                break;
            case MULTIBOOT_TAG_TYPE_MODULE: {
                auto module_tag = reinterpret_cast<mb2_module_tag*>(temp);
                auto modules_array = reinterpret_cast<module*>(reinterpret_cast<uint32_t>(::boot::modules) - KERNEL_PAGE_OFFSET);
                modules_array[module_index].start = module_tag->mod_start;
                modules_array[module_index].end = module_tag->mod_end;
                strcpy(module_tag->name, modules_array[module_index].name);
                ++module_index;
                break;
            }
            case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
                boot_data->elf_symbols = true;
                break;
            case MULTIBOOT_TAG_TYPE_MMAP: {
                const auto size = ((uint32_t*)temp)[1];
                const auto entry_size = ((uint32_t*)temp)[2];
                auto i = 0;
                const auto mmap = reinterpret_cast<memory::mmap*>(
                    reinterpret_cast<uint32_t>(memory::memory_map) - KERNEL_PAGE_OFFSET);
                auto entry = reinterpret_cast<mb2_mmap_entry*>(((uint32_t*)temp + 4));
                for ( ; entry < reinterpret_cast<mb2_mmap_entry*>((char*)temp + size);
                        ++i) {
                    mmap[i].base = static_cast<uint32_t>(entry->base_addr);
                    mmap[i].size = static_cast<uint32_t>(entry->length);
                    mmap[i].type = convert_memory_type(entry->type);
                    entry = reinterpret_cast<mb2_mmap_entry*>((char*)entry + entry_size);
                }
                temp = reinterpret_cast<mb2_tag*>(entry);
                boot_data->memory_map_size = i;
                break;
            }
            case MULTIBOOT_TAG_TYPE_APM:
                boot_data->apm_table_address = (uint32_t)temp + sizeof(uint32_t);
                break;
            default:
                break;
        }
    }
}

} // namespace detail
} // namespace boot

