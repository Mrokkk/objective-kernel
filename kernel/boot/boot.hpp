#pragma once

namespace boot {

struct module {
    uint32_t start, end;
    char name[128];
};

struct data {
    char bootloader_name[32];
    char cmdline[256];
    uint32_t lower_mem;
    uint32_t upper_mem;
    uint32_t modules_start;
    uint32_t modules_end;
    uint32_t mmap_length;
    uint32_t mmap_address;
    uint32_t apm_table_address;
};

extern module modules[];

extern data boot_data;

void print_boot_info();

} // namespace boot

