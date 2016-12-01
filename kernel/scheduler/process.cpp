#include <kernel/scheduler/process.h>
#include <pointer.h>

namespace scheduler {

yacppl::list<yacppl::shared_ptr<process>> processes;

void initialize() {
    auto init_process = yacppl::make_shared<process>();
    processes.push_front(init_process);
}

} // namespace scheduler

