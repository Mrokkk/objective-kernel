#pragma once

#include <stddef.h>

namespace time {

asmlinkage volatile size_t jiffies;
void initialize();

} // namespace time

