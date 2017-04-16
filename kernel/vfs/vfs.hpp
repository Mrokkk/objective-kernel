#pragma once

#include <list.h>
#include <path.h>
#include <array.h>
#include <shared_ptr.h>

#include "block_device.hpp"

namespace vfs {

struct file;
struct vnode;
struct file_system;

using path_t = utils::path;
using vnode_t = utils::shared_ptr<vnode>;
using file_t = utils::shared_ptr<file>;

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

using mount_point_t = utils::shared_ptr<mount_point>;

class vfs {

    static null_block_device null_bd_;

    utils::array<block_device *, 32> block_devices_;
    utils::list<mount_point_t> mount_points_;
    utils::list<vnode_t> vnodes_;
    utils::list<file_t> files_;

public:

    enum class mode {
       read, write, read_write
    };

    vfs(file_system &rootfs, block_device &bd = null_bd_);
    vnode_t mount(const path_t &path, file_system &fs, block_device &bd = null_bd_);
    file_t open(const path_t &path, mode mode);

};

void initialize(file_system &fs, block_device &bd = null_bd);
vnode_t mount_fs(const path_t &path, file_system &fs, block_device &bd = null_bd);

} // namespace vfs

