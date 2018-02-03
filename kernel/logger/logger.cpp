#include <kernel/time/time.hpp>
#include "logger.hpp"

logger logger::instance_("");
utils::spinlock logger::spinlock_;
device::character *logger::device_ = nullptr;
char logger::data_[4096];
size_t logger::index_;

logger::line_wrapper::line_wrapper(logger &logger) : logger_(logger) {
}

logger::line_wrapper::~line_wrapper() {
    logger_ << "\n";
}

logger::logger(const char *component) : component_(component) {
    if (index_ == 0u) {
        *data_ = 0;
    }
}

void logger::set_driver(device::character *device) {
    device_ = device;
    device_->write(data_, utils::length(data_));
}

const char *log_level_to_string(logger::log_level l) {
    switch (l) {
        case logger::log_level::debug: {
            return "DBG";
        }
        case logger::log_level::info: {
            return "INF";
        }
        case logger::log_level::warning: {
            return "WRN";
        }
        case logger::log_level::error: {
            return "ERR";
        }
        default: {
            return "DBG";
        }
    }
}

logger::line_wrapper logger::operator<<(log_level l) {
    char buffer[128];
    sprintf(buffer, "[0x%08x][%s]:%s: ", time::jiffies, log_level_to_string(l), component_);
    print(buffer);
    return line_wrapper(*this);
}

logger &logger::operator<<(const char *str) {
    print(str);
    return *this;
}

logger &logger::operator<<(char str[]) {
    print(str);
    return *this;
}

logger &logger::operator<<(int a) {
    char buf[32];
    sprintf(buf, "%d", a);
    print(buf);
    return *this;
}

logger &logger::operator<<(uint32_t a) {
    char buf[32];
    sprintf(buf, "0x%08x", a);
    print(buf);
    return *this;
}

logger &logger::operator<<(uint64_t a) {
    char buf[32];
    sprintf(buf, "0x%08x", a);
    print(buf);
    return *this;
}

logger &logger::operator<<(uint16_t a) {
    char buf[32];
    sprintf(buf, "0x%04x", a);
    print(buf);
    return *this;
}

logger &logger::operator<<(uint8_t a) {
    char buf[32];
    sprintf(buf, "0x%02x", a);
    print(buf);
    return *this;
}

logger &logger::operator<<(char c) {
    char buf[3]{};
    *buf = c;
    print(buf);
    return *this;
}

