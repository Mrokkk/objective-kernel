#pragma once

namespace drivers {
namespace vga {

void initialize();
void print(const char* text);
int write(const char* buffer, size_t n);

} // namespace vga
} // namespace drivers

