#pragma once

#include "vfs.hpp"
#include "vnode.hpp"
#include "file_system.hpp"

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

    file(const vnode_t &vnode, mode m) : vnode_(vnode), mode_(m) {
    }

    int read(char *buffer, size_t n);
    int write(const char *buffer, size_t n);
    int seek(off_t pos);
    void position(off_t position);
    off_t position() const;
    operator bool() const;

};

file_t open(const path_t &path, file::mode mode = file::mode::read);

} // namespace vfs

