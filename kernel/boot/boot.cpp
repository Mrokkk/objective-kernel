#include <string.hpp>

#include <kernel/logger/logger.hpp>

#include "boot.hpp"
#include "detail/common.hpp"

namespace boot {

namespace {

logger log("boot");

} // namespace

char bootloader_name[128];
char cmdline[128];
uint32_t upper_mem;
module modules[4];
uint32_t modules_start;
uint32_t modules_end;

asmlinkage SECTION(.boot)
void read_bootloader_data(void *data, uint32_t) {
    detail::read_bootloader(data);
}

void initialize() {
    log << logger::info << "Bootloader name: " << boot::bootloader_name;
    log << logger::info << "Boot command-line: " << boot::cmdline;
    log << logger::info << "Upper mem: " << (int)(boot::upper_mem / 1024) << "MiB";
    for (auto i = 0u; modules[i].end != 0; ++i) {
        log << logger::log_level::info << "Module: " << boot::modules[i].name << " @ " << boot::modules[i].start << " - " << boot::modules[i].end;
    }
}

} // namespace boot

