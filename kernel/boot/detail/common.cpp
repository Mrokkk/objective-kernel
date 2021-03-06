#include <kernel/memory/memory.hpp>
#include "../boot.hpp"
#include "common.hpp"

namespace boot {

namespace detail {

SECTION(.boot)
void strcpy(const char *from, char *to) {
    while (*from) {
        *to++ = *from++;
    }
    *to = 0;
}

SECTION(.boot)
uint32_t *phys_address(uint32_t *a) {
    return reinterpret_cast<uint32_t *>(reinterpret_cast<uint32_t>(a) - KERNEL_PAGE_OFFSET);
}

SECTION(.boot)
char *phys_address(char *a) {
    return reinterpret_cast<char *>(reinterpret_cast<uint32_t>(a) - KERNEL_PAGE_OFFSET);
}

SECTION(.boot)
data* boot_data_physical() {
    return reinterpret_cast<data *>(reinterpret_cast<uint32_t>(&boot_data) - KERNEL_PAGE_OFFSET);
}

} // namespace detail

} // namespace boot

