#include "vfs.hpp"
#include "vnode.hpp"
#include "file.hpp"

namespace vfs {

extern utils::unique_ptr<vfs> vfs_;

void initialize(file_system &rootfs, block_device &bd) {
    vfs_ = new vfs(rootfs, bd);
}

vnode_t mount_fs(const path_t &path, file_system &fs, block_device &bd) {
    if (not vfs_) return {};
    return *vfs_->mount(path, fs, bd);
}

cache &get_cache() {
    return vfs_->get_cache();
}

vnode_t lookup(const path_t &path) {
    if (not vfs_) return {};
    auto result = vfs_->lookup(path);
    if (not result) return {};
    return result.operator*();
}

vnode_t create(const path_t &path, vnode::type type) {
    if (not vfs_) return {};
    auto result = vfs_->create(path, type);
    if (not result) return {};
    return *result;
}

file_t open(const path_t &path, file::mode mode) {
    if (not vfs_) return {};
    auto result = vfs_->open(path, mode);
    if (not result) return {};
    return *result;
}

} // namespace vfs
