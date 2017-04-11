#pragma once

#include <path.h>
#include <shared_ptr.h>

#include "block_device.hpp"

namespace vfs {

struct file;
struct vnode;
struct file_system;

using vnode_t = utils::shared_ptr<vnode>;
using path_t = utils::path;

struct mount_point final {

    path_t path;
    file_system *fs;
    dev_t dev;

    mount_point(const path_t &path, file_system &fs, dev_t dev)
            : path(path), fs(&fs), dev(dev) {
    }

    ~mount_point() {
    }

};

void initialize(file_system &fs, block_device &bd = null_bd);
vnode_t mount_fs(const path_t &path, file_system &fs, block_device &bd = null_bd);

} // namespace vfs

