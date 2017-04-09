#pragma once

#include <path.h>
#include <shared_ptr.h>

#include "block_device.hpp"

namespace vfs {

struct file;
struct vnode;

using dev_t = short;
using vnode_t = utils::shared_ptr<vnode>;
using path_t = utils::path;

struct file_system {
    virtual utils::string name() = 0;
    virtual vnode_t lookup(const path_t &path) = 0;
    virtual vnode_t create(const path_t &path) = 0;
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

    vnode(unsigned i, size_t s, size_t b, uint32_t d, file_system *f)
            : id(i), size(s), blocks(b), data(d), fs(f) {
    }

};

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

struct mount_point final {

    path_t path;
    file_system *fs;
    dev_t dev;

    mount_point(const path_t &path, file_system &fs, dev_t dev)
            : path(path), fs(&fs), dev(dev) {
    }

    ~mount_point() {
    }

};

extern null_block_device null;

void initialize(file_system &fs, block_device &bd = null);
file open(const path_t &path, file::mode mode = file::mode::read);
int register_device(block_device &dev);
void mount_fs(const path_t path, file_system &fs);
utils::shared_ptr<vnode> lookup(const path_t &path);

} // namespace vfs

