#include <kernel/logger/logger.hpp>

#include "boot.hpp"
#include "detail/common.hpp"

namespace boot {

namespace {
logger log("boot");
} // namespace

data boot_data;
module modules[4];

void print_boot_info() {
    log << logger::info << "Bootloader name: " << boot_data.bootloader_name;
    log << logger::info << "Boot command-line: " << boot_data.cmdline;
    log << logger::info << "Lower mem: " << (int)(boot_data.lower_mem) << "kiB";
    log << logger::info << "Upper mem: " << (int)(boot_data.upper_mem / 1024) << "MiB";
    log << logger::info << "Memory map size: " << (int)boot_data.memory_map_size;
    log << logger::info << "APM table addr: " << boot_data.apm_table_address;
    log << logger::info << "ELF symbols available: " << boot_data.elf_symbols;
    for (auto i = 0u; modules[i].end != 0; ++i) {
        log << logger::log_level::info << "Module: " << boot::modules[i].name << " @ " << boot::modules[i].start << " - " << boot::modules[i].end;
    }
}

} // namespace boot

