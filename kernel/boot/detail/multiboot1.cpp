#include <kernel/memory/paging/paging.hpp>

#include "../boot.hpp"
#include "../multiboot.hpp"
#include "common.hpp"

namespace boot {

namespace detail {

SECTION(.boot)
void read_bootloader(void *tag) {
    auto mb_data = static_cast<multiboot_info *>(tag);
    strcpy(mb_data->cmdline, phys_address(::boot::cmdline));
    strcpy(mb_data->bootloader_name, phys_address(::boot::bootloader_name));
    *phys_address(&::boot::upper_mem) = mb_data->mem_upper;
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

