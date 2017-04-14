#include <list.h>
#include "file.hpp"

namespace vfs {

utils::list<file_t> files;

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

int file::seek(off_t pos) {
    position_ = pos;
    return 0;
}

file::operator bool() const {
    return vnode_ ? true : false;
}

void file::position(off_t position) {
    position_ = position;
}

off_t file::position() const {
    return position_;
}

size_t file::size() const {
    return vnode_->size;
}

file_t open(const path_t &path, file::mode mode) {
    // TODO: creating file
    auto node = lookup(path);
    if (not node) {
        return {};
    }
    return utils::make_shared<file>(node.get(), mode);
}

} // namespace vfs

