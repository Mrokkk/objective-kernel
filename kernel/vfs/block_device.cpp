#include "block_device.hpp"

namespace vfs {

struct NullBlockDevice : public IBlockDevice {

    const char *name() override {
        return "name";
    }

    void handle_request(IBlockDevice::Request *) override {
    }

};

} // namespace vfs
