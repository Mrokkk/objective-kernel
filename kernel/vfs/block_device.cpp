#include "block_device.hpp"

namespace vfs {

struct NullBlockDevice : public block_device {

    const char *name() override {
        return "name";
    }

    void handle_request(block_device::request *) override {
    }

};

} // namespace vfs
