#pragma once

#include <kernel/device/character.hpp>

namespace drivers {

namespace vga {

struct driver {
    explicit driver(size_t dev_id);
    int write(const char *, size_t);
};

void initialize();
void print(const char* text);
int write(const char* buffer, size_t n);

} // namespace vga

} // namespace drivers

