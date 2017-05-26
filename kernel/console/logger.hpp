#pragma once

#include <string.hpp>
#include <algorithm.hpp>

#include "console.hpp"

namespace console {

#define assert(cond) \
    do { \
        if (!(cond)) { \
            ::console::cout << (utils::last_occurrence(__FILE__, '/') + 1) << ":" << __LINE__ << "WRN:" << "assertion failed: " << #cond << "\n"; \
        } \
    } while (0)

} // namespace console

