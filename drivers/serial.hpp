#pragma once

namespace drivers {

namespace serial {

int initialize();
void print(const char *string);
int write(const char *buffer, size_t n);

} // namespace serial

} // namespace drivers

