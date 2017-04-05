#pragma once

#include "block_device.hpp"

namespace vfs {

struct SuperBlock {
    // TODO
};

struct IFileSystem {
    virtual const char *name() = 0;
    virtual SuperBlock *read_super_block() = 0;
};

void initialize();
void register_device(IBlockDevice &dev);

} // namespace vfs

