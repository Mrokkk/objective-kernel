#include <list.h>
#include "file.hpp"

namespace vfs {

extern utils::list<vnode_t> vnodes;

int file::read(char *buffer, size_t n) {
    if (mode_ == mode::write) {
        return -1;
    }
    return vnode_->fs->read(*vnode_, buffer, n);
}

int file::write(const char *buffer, size_t n) {
    if (mode_ == mode::read) {
        return -1;
    }
    return vnode_->fs->write(*vnode_, buffer, n);
}

int file::seek(int pos) {
    position_ = pos;
    return 0;
}

file::operator bool() {
    if (vnode_) return true;
    return false;
}

file open(const path_t &path, file::mode mode) {
    auto node = lookup(path);
    if (!node) {
        return {};
    }
    vnodes.push_back(node);
    return file(vnodes.back().get(), mode);
}


} // namespace vfs

