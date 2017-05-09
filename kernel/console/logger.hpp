#pragma once

#include <string.hpp>
#include <algorithm.hpp>

#include "console.hpp"

namespace console {

#define debug "DEBUG: "
#define warning "WARNING: "

#define assert(cond) \
    do { \
        if (!(cond)) { \
            ::console::cout << (utils::last_occurrence(__FILE__, '/') + 1) << ":" << __LINE__ << warning << "assertion failed: " << #cond << "\n"; \
        } \
    } while (0)

} // namespace console

