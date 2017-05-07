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

