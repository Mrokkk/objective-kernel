#pragma once

#include <path.h>
#include "block_device.hpp"

namespace vfs {

struct file;

struct file_system final {

    struct low_interface {
        virtual int lookup() = 0;
        virtual int read() = 0;
        virtual int write() = 0;
    };

private:

    low_interface &interface_;

public:

    file_system(low_interface &interface) : interface_(interface) {
    }

    auto read() {
        return interface_.read();
    }

    int mount(const utils::path &) {
        return 0;
    }

};

struct vnode final {

    enum class type {
        file, dir
    };

    size_t size = 0;
    size_t blocks = 0;
    uint32_t data = 0;
    file_system *fs = nullptr;

    vnode() = default;

    vnode(size_t s, size_t b, uint32_t d) : size(s), blocks(b), data(d) {
    }

};

struct file final {

    enum class mode {
        read, write, read_write
    };

private:

    vnode &vnode_;
    mode mode_;
    off_t position_;

public:

    explicit file(vnode &vnode, mode m) : vnode_(vnode), mode_(m) {
    }

    int read(char *, size_t) {
        if (mode_ == mode::write) {
            return 1;
        }
        vnode_.fs->read();
        return 0;
    }

    int write(const char *, size_t) {
        if (mode_ == mode::read) {
            return 1;
        }
        return 0;
    }

    int seek(int pos) {
        position_ = pos;
        return 0;
    }

};

struct mount_point {

    utils::path path;
    file_system &fs;

    ~mount_point() {
    }

};

void initialize();
void register_device(block_device &dev);
void mount_fs(const char *path, file_system &fs);

} // namespace vfs

