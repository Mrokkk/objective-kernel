#pragma once

namespace boot {

struct module {
    uint32_t start, end;
    char name[128];
};

struct memory_region {
    uint32_t base;
    uint32_t size;
    uint32_t type;
};

struct data {
    char bootloader_name[32];
    char cmdline[256];
    uint32_t lower_mem;
    uint32_t upper_mem;
    uint32_t modules_start;
    uint32_t modules_end;
    size_t memory_map_size;
    memory_region memory_map[10];
    uint32_t apm_table_address;
    bool elf_symbols;
};

extern module modules[];

extern data boot_data;

void print_boot_info();

} // namespace boot

