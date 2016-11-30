#pragma once

namespace console {

using printer_function = void (*)(const char *);

void initialize(printer_function);
void print(const char *fmt, ...);

} // namespace console

