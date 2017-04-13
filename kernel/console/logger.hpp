#pragma once

#include <string.h>
#include <algorithm.h>

#include "console.hpp"

namespace console {

#define warning(...) \
    console::print(utils::last_occurrence(__FILE__, '/') + 1, ":", __LINE__, ":WARNING: ", __VA_ARGS__, "\n")

#define debug(...) \
    console::print(utils::last_occurrence(__FILE__, '/') + 1, ":", __LINE__, ":DEBUG: ", __VA_ARGS__, "\n")

} // namespace console

