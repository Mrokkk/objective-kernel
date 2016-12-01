#include <kernel/scheduler/process.h>
#include <shared_ptr.h>

namespace scheduler {

utils::list<utils::shared_ptr<process>> processes;

void initialize() {
    auto init_process = utils::make_shared<process>();
    processes.push_front(init_process);
}

} // namespace scheduler

