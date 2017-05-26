#pragma once

#include <list.hpp>
#include <array.hpp>

#include <kernel/error.hpp>
#include <kernel/logger/logger.hpp>

#include "file.hpp"
#include "cache.hpp"
#include "definitions.hpp"
#include "block_device.hpp"
#include "mount_point.hpp"

namespace vfs {

class vfs {

    static null_block_device null_bd_;
    logger logger_;

    utils::array<block_device *, 32> block_devices_;
    unsigned bd_index_ = 0;
    utils::list<mount_point_t> mount_points_;
    utils::list<file_t> files_;
    cache cache_;

    bool node_exists(const path_t &filename, const vnode_t &parent) const;
    void get_vnode(const utils::string &name, vnode_t &parent_node, cache::dir_entry *parent_entry, cache::dir_entry *child_entry);

public:

    enum class mode {
       read, write, read_write
    };

    vfs(file_system &rootfs, block_device &bd);
    maybe<vnode_t> mount(const path_t &path, file_system &fs, block_device &bd);
    maybe<vnode_t> lookup(const path_t &path);
    maybe<vnode_t> create(const path_t &path, vnode::type type);
    maybe<file_t> open(const path_t &path, file::mode mode);
    cache &get_cache();

    int register_device(block_device &dev);
    dev_t get_device_id(block_device &bd);

};

void initialize(file_system &fs, block_device &bd = null_bd_);
vnode_t mount_fs(const path_t &path, file_system &fs, block_device &bd = null_bd_);
cache &get_cache();

} // namespace vfs

