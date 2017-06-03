#pragma once

#include "vnode.hpp"
#include "definitions.hpp"
#include "file_system.hpp"

namespace vfs {

struct file final {

    enum class mode {
        read, write, read_write
    };

    mode mode_ = mode::read;
private:

    vnode_t vnode_;
    off_t position_ = 0;

public:

    file(const vnode_t &vnode, mode mode) : mode_(mode), vnode_(vnode) {
    }

    int read(char *buffer, size_t n);
    int write(const char *buffer, size_t n);
    int seek(off_t pos);
    void position(off_t position);
    off_t position() const;
    size_t size() const;
    operator bool() const;

};

} // namespace vfs

