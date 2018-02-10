#include <drivers/serial.hpp>
#include <string.hpp>
#include <kernel/boot/boot.hpp>
#include <kernel/logger/logger.hpp>
#include "cmdline_parser.hpp"
#include "kernel.hpp"

namespace {

void handle(const yacppl::string& entry) {
    logger log("parse_cmdline");
    const auto splitted = entry.split('=');
    if (splitted.size() < 1) return;
    const auto& key = splitted[0];
    const auto& value = splitted[1];
    if (key == "syslog") {
        log << logger::debug << "setting syslog to " << value;
        auto dev = kernel::device_manager().get_character_device(value);
        if (not dev) {
            log << logger::error << "no such device " << value;
            return;
        }
        logger::set_driver(dev);
    }
    else {
        log << logger::error << "no such key: " << key;
    }
}

} // namespace

void parse_cmdline() {
    yacppl::string cmdline(boot::boot_data.cmdline);
    const auto vec = cmdline.split();
    for (const auto& e : vec) {
        handle(e);
    }
}

