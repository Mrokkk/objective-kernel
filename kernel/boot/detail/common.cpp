#include <kernel/memory/paging/paging.hpp>
#include "common.hpp"

namespace boot {

namespace detail {

__section(.boot)
void strcpy(const char *from, char *to) {
    while (*from) {
        *to++ = *from++;
    }
    *to = 0;
}

__section(.boot)
uint32_t *phys_address(uint32_t *a) {
    return reinterpret_cast<uint32_t *>(reinterpret_cast<uint32_t>(a) - KERNEL_PAGE_OFFSET);
}

__section(.boot)
char *phys_address(char *a) {
    return reinterpret_cast<char *>(reinterpret_cast<uint32_t>(a) - KERNEL_PAGE_OFFSET);
}

} // namespace detail

} // namespace boot

