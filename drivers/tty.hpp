#pragma once

#include <kernel/device/character.hpp>

namespace drivers {

namespace tty {

struct driver final : public device::character {

    using read_t = int (*)(char *, size_t);
    using write_t = int (*)(const char *, size_t);

    explicit driver(const read_t, const write_t, const device::character::id_t);

    const char *name() override {
        return "tty";
    }

    int read(char *, size_t) override;
    int write(const char *, size_t) override;

private:
    const read_t read_;
    const write_t write_;
};

void initialize();

} // namespace tty

} // namespace drivers

