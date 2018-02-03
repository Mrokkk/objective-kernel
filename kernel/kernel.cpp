#include "interfaces/component.hpp"
#include "interfaces/device_manager.hpp"
#include "kernel.hpp"
#include "time/time.hpp"
#include "boot/boot.hpp"

namespace kernel {

kernel::kernel() : components_(&interfaces::component::node_), logger_("kernel") {
}

void kernel::register_component(interfaces::component &c) {
    components_.push_back(c);
}

void kernel::run() {
    for (auto &c : components_) {
        logger_ << logger::info << "Initializing " << c.name_;
        c.initialize();
    }
    boot::print_boot_info();
    time::initialize();
    cpu::sti();
    while (1) {
        cpu::halt();
    }
}

} // namespace kernel
