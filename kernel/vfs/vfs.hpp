#pragma once

#include <path.h>
#include <shared_ptr.h>

#include "block_device.hpp"

namespace vfs {

struct file;
struct vnode;

using dev_t = short;
using vnode_t = utils::shared_ptr<vnode>;
using path_t = utils::path;

struct file_system {
    virtual utils::string name() = 0;
    virtual vnode_t lookup(const path_t &path) = 0;
    virtual vnode_t create(const path_t &path) = 0;
    virtual int read(vnode &vnode, char *buffer, size_t size = 0) = 0;
    virtual int write(vfs::vnode &vnode, const char *buffer, size_t size) = 0;
};

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

extern null_block_device null;

void initialize(file_system &fs, block_device &bd = null);
int register_device(block_device &dev);
void mount_fs(const path_t path, file_system &fs);

} // namespace vfs

