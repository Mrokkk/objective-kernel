#include <shared_ptr.hpp>
#include <kernel/scheduler/process.hpp>

namespace scheduler {

utils::list<utils::shared_ptr<process>> processes;

void initialize() {
    auto init_process = utils::make_shared<process>();
    processes.push_front(init_process);
}

} // namespace scheduler

