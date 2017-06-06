#include "kernel.hpp"
#include "component.hpp"

namespace kernel {

kernel::kernel() : components_(&component::node_), logger_("kernel") {
}

void kernel::register_component(component &c) {
    components_.push_back(c);
}

void kernel::run() {
    for (auto &c : components_) {
        logger_ << logger::log_level::info << "Initializing " << c.name_;
        c.initialize();
    }
    cpu::sti();
    while (1) {
        cpu::halt();
    }
}

} // namespace kernel
