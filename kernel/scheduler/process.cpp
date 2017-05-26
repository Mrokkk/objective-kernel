#include <shared_ptr.hpp>
#include <kernel/logger/logger.hpp>
#include "process.hpp"

namespace scheduler {

utils::list<utils::shared_ptr<process>> processes;
logger log;

void initialize() {
    log.initialize("scheduler");
    log << "Initializing...\n";
    auto init_process = utils::make_shared<process>();
    processes.push_front(init_process);
}

} // namespace scheduler

