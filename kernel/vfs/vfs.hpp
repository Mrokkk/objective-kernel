#pragma once

namespace vfs {

// interface fo block devices
struct IBlock_device {

    // interface for block device iterator
    struct IIterator {
    };

};

class fs {

    IBlock_device device_;

public:

    fs(IBlock_device &device)
            : device_(device) {
    }

};

// file interface
struct IFile {
    virtual void close() = 0;
    virtual void read() = 0;
    virtual void write() = 0;
};

struct file : public IFile {
};

// fs node
struct IVnode {
};

struct vnode : public IVnode {
};

} // namespace vfs

