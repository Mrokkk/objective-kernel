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
}

} // namespace detail

} // namespace boot

