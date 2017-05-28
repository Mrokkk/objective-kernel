#include <shared_ptr.hpp>
#include <kernel/logger/logger.hpp>
#include "process.hpp"

namespace scheduler {

utils::list<utils::shared_ptr<process>> processes;
logger log("scheduler");

void initialize() {
    log << logger::log_level::debug << "Initializing...";
    auto init_process = utils::make_shared<process>();
    processes.push_front(init_process);
}

} // namespace scheduler

