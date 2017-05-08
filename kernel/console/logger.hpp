#pragma once

#include <string.h>
#include <algorithm.h>

#include "console.hpp"

namespace console {

#define warning(...) \
    //console::cout << (utils::last_occurrence(__FILE__, '/') + 1) << ":" << __LINE__ << ":WARNING: " << __VA_ARGS__ << "\n"

#define debug(...) \
    //console::cout << (utils::last_occurrence(__FILE__, '/') + 1) << ":" << __LINE__ << ":DEBUG: " << __VA_ARGS__ << "\n"

#define assert(cond) \
    do { \
        if (!(cond)) { \
            warning("assertion failed: ", #cond); \
        } \
    } while (0)

} // namespace console

