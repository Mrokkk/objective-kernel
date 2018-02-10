#include <drivers/vga.hpp>
#include <drivers/serial.hpp>
#include "boot/boot.hpp"
#include "cmdline_parser.hpp"
#include "interfaces/component.hpp"
#include "interfaces/device_manager.hpp"
#include "kernel.hpp"
#include "time/time.hpp"

interfaces::device_manager* kernel::device_manager_;
interfaces::interrupt_manager* kernel::interrupt_manager_;
interfaces::scheduler* kernel::scheduler_;

kernel::kernel()
        : logger_("kernel") {
}

void kernel::register_device_manager(interfaces::device_manager& device_manager) {
    device_manager_ = &device_manager;
}

void kernel::register_interrupt_manager(interfaces::interrupt_manager& interrupt_manager) {
    interrupt_manager_ = &interrupt_manager;
}

void kernel::register_scheduler(interfaces::scheduler& scheduler) {
    scheduler_ = &scheduler;
}

interfaces::interrupt_manager& kernel::interrupt_manager() {
    return *interrupt_manager_;
}

interfaces::device_manager& kernel::device_manager() {
    return *device_manager_;
}

interfaces::scheduler& kernel::scheduler() {
    return *scheduler_;
}

NORETURN void kernel::run() {
    interrupt_manager_->initialize();
    scheduler_->initialize();
    device_manager_->initialize();
    boot::print_boot_info();
    parse_cmdline();
    time::initialize();
    cpu::sti();
    logger_ << logger::info << "enabled interrupts";
    while (1) {
        cpu::halt();
    }
}

