#include "vfs.hpp"

namespace vfs {

class DummyFs : public IFileSystem {

    SuperBlock sb_;
    const char *name_ = "dummy";

public:

    const char *name() override {
        return name_;
    }

    SuperBlock *read_super_block() override {
        return &sb_;
    }

};

void initialize() {
}

void register_device(IBlockDevice &) {
}

void mount_fs(const char *, IFileSystem &) {
    // TODO
}

} // namespace vfs

