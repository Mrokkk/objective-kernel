#include <string.h>

#include <kernel/memory/paging/paging.hpp>

#include "boot.hpp"
#include "multiboot.hpp"

namespace boot {

char bootloader_name[128];
char cmdline[128];
uint32_t upper_mem;
module modules[4];
uint32_t modules_start;
uint32_t modules_end;

namespace detail {

void read_bootloader(void *data);

} // namespace detail

asmlinkage __section(.text.boot)
void read_bootloader_data(void *data, uint32_t) {
    detail::read_bootloader(data);
}

} // namespace bootloader
