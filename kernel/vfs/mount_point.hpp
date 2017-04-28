#pragma once

#include <hash_map.h>
#include "definitions.hpp"
#include "block_device.hpp"

namespace vfs {

class mount_point final {


public:
    path_t path;
    file_system *fs;
    dev_t dev;
    utils::list<vnode_t> vnodes_;

    mount_point(const path_t &path, file_system &fs, dev_t dev)
            : path(path), fs(&fs), dev(dev) {
    }

    ~mount_point() {
    }

};



} // namespace vfs

