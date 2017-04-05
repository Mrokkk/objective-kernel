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

struct MountPoint {
    // TODO
    const char *path;
    IFileSystem &fs;
    SuperBlock &sb;

    ~MountPoint() {
        // release superblock
    }

};

void initialize();
void register_device(IBlockDevice &dev);
void mount_fs(const char *path, IFileSystem &fs);

} // namespace vfs

