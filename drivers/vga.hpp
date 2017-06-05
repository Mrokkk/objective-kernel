#pragma once

#include <kernel/device/character.hpp>

namespace drivers {

namespace vga {

struct device : public ::device::character {
    const char *name() override;
    int write(const char *buffer, size_t n) override;
};

void initialize();
void print(const char *text);

} // namespace vga

} // namespace drivers

