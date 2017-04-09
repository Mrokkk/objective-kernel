#pragma once

#include <path.h>
#include "block_device.hpp"

namespace vfs {

struct file;
struct vnode;

using dev_t = short;

struct file_system {
    virtual vnode lookup(const utils::path &path) = 0;
    virtual vfs::vnode create(const utils::path &path) = 0;
    virtual int read(vnode &vnode, char *buffer, size_t size = 0) = 0;
    virtual int write(vfs::vnode &vnode, const char *buffer, size_t size) = 0;
};

struct vnode final {

    enum class type {
        file, dir
    };

    unsigned id = 0;
    size_t size = 0;
    size_t blocks = 0;
    uint32_t data = 0;
    file_system *fs = nullptr;

    vnode() = default;

    vnode(unsigned i, size_t s, size_t b, uint32_t d, file_system *f)
            : id(i), size(s), blocks(b), data(d), fs(f) {
    }

};

struct file final {

    enum class mode {
        read, write, read_write
    };

private:

    vnode *vnode_ = nullptr;
    mode mode_ = mode::read;
    off_t position_ = 0;;

public:

    file() = default;

    file(vnode *vnode, mode m) : vnode_(vnode), mode_(m) {
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

struct mount_point {

    utils::path path;
    file_system *fs;
    dev_t dev;

    mount_point(const utils::path &path, file_system &fs, dev_t dev)
            : path(path), fs(&fs), dev(dev) {
    }

    ~mount_point() {
    }

};

extern null_block_device null;

void initialize(file_system &, block_device & = null);
file open(const utils::path &path, file::mode mode = file::mode::read);
int register_device(block_device &dev);
void mount_fs(const char *path, file_system &fs);

} // namespace vfs

