#pragma once

namespace boot {

namespace detail {

void strcpy(const char *from, char *to);
uint32_t *phys_address(uint32_t *a);
char *phys_address(char *a);

} // namespace detail

} // namespace boot

