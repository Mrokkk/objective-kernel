#pragma once

#include <kernel/device/character.hpp>

namespace drivers {

namespace vga {


void initialize();
void print(const char *text);

} // namespace vga

} // namespace drivers

