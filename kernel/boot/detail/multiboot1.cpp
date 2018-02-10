#include <kernel/cpu/apm.hpp>
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

SECTION(.boot)
void read_mmap(uint32_t, uint32_t mmap_address) {
    memory_map *mm;
    int i;
    const auto mmap = reinterpret_cast<memory::mmap *>(
        reinterpret_cast<uint32_t>(memory::memory_map) - KERNEL_PAGE_OFFSET);
    for (mm = (struct memory_map *)mmap_address, i = 0;
         mm->base_addr_low + (mm->length_low - 1) != 0xffffffff;
         ++i) {
        mmap[i].type = convert_memory_type(mm->type);
        mmap[i].base = mm->base_addr_low;
        mmap[i].size = mm->length_low;
        mm = (memory_map *)((unsigned int)mm + mm->size + 4);
    }
    boot_data_physical()->memory_map_size = i;
}

asmlinkage SECTION(.boot)
void read_bootloader_data(void *tag) {
    auto boot_data = boot_data_physical();
    const auto mb_data = static_cast<multiboot_info *>(tag);
    strcpy(mb_data->cmdline, boot_data->cmdline);
    strcpy(mb_data->bootloader_name, boot_data->bootloader_name);
    boot_data->lower_mem = mb_data->mem_lower;
    boot_data->upper_mem = mb_data->mem_upper;
    read_mmap(mb_data->mmap_length, mb_data->mmap_addr);
    boot_data->apm_table_address = mb_data->apm_table;
    if (mb_data->flags & (1 << 3) && mb_data->mods_count) {
        auto modules_array = reinterpret_cast<module *>(reinterpret_cast<uint32_t>(::boot::modules) - KERNEL_PAGE_OFFSET);
        auto module_ptr = reinterpret_cast<::module *>(mb_data->mods_addr);
        for (auto module_index = 0u; module_index < mb_data->mods_count; module_index++, module_ptr++) {
            modules_array[module_index].start = module_ptr->mod_start;
            modules_array[module_index].end = module_ptr->mod_end;
            strcpy(module_ptr->string, modules_array[module_index].name);
        }
    }
}

} // namespace detail

} // namespace boot

