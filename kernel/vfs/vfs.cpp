#include <list.h>
#include "vfs.hpp"

namespace vfs {

mount_point root;
null_block_device null;
utils::list<vnode> node_list;

void initialize(file_system &rootfs, block_device &) {
    root.path = utils::path("/");
    root.fs = &rootfs;
}

file open(const utils::path &path, file::mode mode) {
    auto node = root.fs->lookup(path);
    if (node.fs == nullptr) {
        return {};
    }
    node_list.push_back(node);
    return file(const_cast<vnode *>(&node_list.back()), mode);
}

void register_device(block_device &) {
}

void mount_fs(const char *, file_system &) {
    // TODO
}

} // namespace vfs

