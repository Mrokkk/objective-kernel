#include <array.h>
#include "vfs.hpp"
#include "block_device.hpp"

namespace vfs {

utils::array<block_device *, 32> block_devices;
null_block_device null_bd;
unsigned bd_index = 0;

int register_device(block_device &bd) {
    block_devices[bd_index++] = &bd;
    return bd_index - 1;
}

dev_t get_device_id(block_device &bd) {
    for (auto i = 0u; i < bd_index; ++i) {
        if (block_devices[i] == &bd) {
            return i;
        }
    }
    return -1;
}

} // namespace vfs
