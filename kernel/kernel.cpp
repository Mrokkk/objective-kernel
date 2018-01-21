#include "kernel.hpp"
#include "interfaces/component.hpp"

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
    cpu::sti();
    while (1) {
        cpu::halt();
    }
}

} // namespace kernel
