#include <list.h>
#include "file.hpp"

namespace vfs {

int file::read(char *buffer, size_t n) {
    if (mode_ == mode::write) {
        return -1;
    }
    return vnode_->fs->read(this, vnode_, buffer, n);
}

int file::write(const char *buffer, size_t n) {
    if (mode_ == mode::read) {
        return -1;
    }
    return vnode_->fs->write(this, vnode_, buffer, n);
}

int file::seek(int pos) {
    position_ = pos;
    return 0;
}

file::operator bool() const {
    return vnode_ ? true : false;
}

file open(const path_t &path, file::mode mode) {
    // TODO: creating file
    auto node = lookup(path);
    if (!node) {
        return {};
    }
    return file(node.get(), mode);
}

} // namespace vfs

