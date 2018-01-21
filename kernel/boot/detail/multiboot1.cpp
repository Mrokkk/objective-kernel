#include <kernel/cpu/apm.hpp>
#include <kernel/memory/memory.hpp>

#include "../boot.hpp"
#include "../multiboot.hpp"
#include "common.hpp"

namespace boot {

namespace detail {

SECTION(.boot)
void read_mmap(uint32_t , uint32_t mmap_address) {
    memory_map *mm;
    int i;

    const auto mmap = reinterpret_cast<memory::mmap *>(
        reinterpret_cast<uint32_t>(memory::memory_map) - KERNEL_PAGE_OFFSET);
    for (mm = (struct memory_map *)mmap_address, i = 0;
         mm->base_addr_low + (mm->length_low - 1) != 0xffffffff;
         i++)
    {
        mm = (memory_map *)((unsigned int)mm + mm->size + 4);
        switch (mm->type) {
            case 1:
                mmap[i].type = memory::mmap::type::available;
                break;
            case 2:
                mmap[i].type = memory::mmap::type::unavailable;
                break;
            case 3:
                mmap[i].type = memory::mmap::type::device;
                break;
            case 4:
                mmap[i].type = memory::mmap::type::device;
                break;
            default:
                mmap[i].type = memory::mmap::type::unknown;
        }

        mmap[i].base = mm->base_addr_low;
        mmap[i].size = mm->length_low;
    }

    mmap[i].base = 0;
}

SECTION(.boot)
void read_bootloader(void *tag) {
    const auto mb_data = static_cast<multiboot_info *>(tag);
    strcpy(mb_data->cmdline, phys_address(::boot::cmdline));
    strcpy(mb_data->bootloader_name, phys_address(::boot::bootloader_name));
    *phys_address(&::boot::lower_mem) = mb_data->mem_lower;
    *phys_address(&::boot::upper_mem) = mb_data->mem_upper;
    *phys_address(&::boot::mmap_length) = mb_data->mmap_length;
    *phys_address(&::boot::mmap_address) = mb_data->mmap_addr;
    read_mmap(mb_data->mmap_length, mb_data->mmap_addr);
    *phys_address(&::boot::apm_table_address) = mb_data->apm_table;
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

