#include <string.hpp>

#include <kernel/logger/logger.hpp>
#include <kernel/cpu/apm.hpp>

#include "boot.hpp"
#include "detail/common.hpp"

namespace boot {

namespace {

logger log("boot");

} // namespace

char bootloader_name[128];
char cmdline[128];
uint32_t lower_mem;
uint32_t upper_mem;
module modules[4];
uint32_t modules_start;
uint32_t modules_end;
uint32_t mmap_length;
uint32_t mmap_address;
uint32_t apm_table_address;

asmlinkage SECTION(.boot)
void read_bootloader_data(void *data, uint32_t) {
    detail::read_bootloader(data);
}

void initialize() {
    log << logger::info << "Bootloader name: " << boot::bootloader_name;
    log << logger::info << "Boot command-line: " << boot::cmdline;
    log << logger::info << "Lower mem: " << (int)(boot::lower_mem) << "kiB";
    log << logger::info << "Upper mem: " << (int)(boot::upper_mem / 1024) << "MiB";
    log << logger::info << "Memory map len: " << (int)(boot::mmap_length);
    log << logger::info << "Memory map addr: " << boot::mmap_address;
    log << logger::info << "APM table addr: " << boot::apm_table_address;
    for (auto i = 0u; modules[i].end != 0; ++i) {
        log << logger::log_level::info << "Module: " << boot::modules[i].name << " @ " << boot::modules[i].start << " - " << boot::modules[i].end;
    }
}

} // namespace boot

