#include <list.h>
#include <unique_ptr.h>

#include "file.hpp"
#include "vfs.hpp"

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

extern utils::unique_ptr<vfs> vfs_;

file_t open(const path_t &path, file::mode mode) {
    if (not vfs_) return {};
    auto result = vfs_->open(path, mode);
    if (not result) return {};
    return *result;
}

} // namespace vfs

