#include "block_device.hpp"

namespace interfaces {

int block_device::read(char*, size_t) {
    return -1;
}

int block_device::write(const char*, size_t) {
    return -1;
}

} // namespace interfaces

