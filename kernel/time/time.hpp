#pragma once

#include <cstddef>

namespace time {

asmlinkage volatile size_t jiffies;
void initialize();

} // namespace time

