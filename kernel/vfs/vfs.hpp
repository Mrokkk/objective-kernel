#pragma once

#include <path.h>
#include "block_device.hpp"

namespace vfs {

struct vnode final {
};

struct file final {

    enum class mode {
        read, write, read_write
    };

    struct operations {
        virtual int read() = 0;
        virtual int write() = 0;
    };

private:

    operations &ops_;
    mode mode_;
    off_t position_;

public:

    explicit file(operations &ops, mode m) : ops_(ops), mode_(m) {
    }

    int read(char *, size_t) {
        if (mode_ == mode::write) {
            return 1;
        }
        ops_.read();
        return 0;
    }

    int write(const char *, size_t) {
        if (mode_ == mode::read) {
            return 1;
        }
        ops_.write();
        return 0;
    }

    int seek(int pos) {
        position_ = pos;
        return 0;
    }

};

struct super_block final {
};

struct file_system {
    virtual const char *name() = 0;
};

struct mount_point {

    utils::path path;
    file_system &fs;
    //super_block &sb;

    ~mount_point() {
    }

};

void initialize();
void register_device(block_device &dev);
void mount_fs(const char *path, file_system &fs);

} // namespace vfs

