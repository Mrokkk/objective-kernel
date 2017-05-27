#include <kernel/cpu/pit.hpp>
#include "time.hpp"

namespace time {

volatile size_t jiffies;

void initialize() {
    cpu::pit::initialize();
}

} // namespace time

