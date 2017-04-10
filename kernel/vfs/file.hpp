#pragma once

#include "vfs.hpp"
#include "vnode.hpp"

namespace vfs {

struct file final {

    enum class mode {
        read, write, read_write
    };

private:

    vnode_t vnode_ = nullptr;
    mode mode_ = mode::read;
    off_t position_ = 0;

public:

    file() = default;

    file(const vnode_t &vnode, mode m) : vnode_(vnode), mode_(m) {
    }

    int read(char *buffer, size_t n) {
        if (mode_ == mode::write) {
            return -1;
        }
        return vnode_->fs->read(*vnode_, buffer, n);
    }

    int write(const char *buffer, size_t n) {
        if (mode_ == mode::read) {
            return -1;
        }
        return vnode_->fs->write(*vnode_, buffer, n);
    }

    int seek(int pos) {
        position_ = pos;
        return 0;
    }

    operator bool() {
        if (vnode_) return true;
        return false;
    }

};

file open(const path_t &path, file::mode mode = file::mode::read);

} // namespace vfs

