#include "vfs.hpp"

namespace vfs {

class DummyFs : public file_system {

    super_block sb_;
    const char *name_ = "dummy";

public:

    const char *name() override {
        return name_;
    }

};

void initialize() {
}

void register_device(block_device &) {
}

void mount_fs(const char *, file_system &) {
    // TODO
}

} // namespace vfs

