#pragma once

namespace boot {

struct data;

namespace detail {

void strcpy(const char* from, char* to);
uint32_t* phys_address(uint32_t *a);
char* phys_address(char *a);
data* boot_data_physical();

} // namespace detail

} // namespace boot

