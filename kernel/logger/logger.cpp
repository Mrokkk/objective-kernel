#include <kernel/time/time.hpp>
#include "logger.hpp"

logger logger::instance_("");
console::console *logger::console_;
utils::spinlock logger::spinlock_;
console::console logger::default_;
char logger::data_[4096];
size_t logger::index_;

logger::line_wrapper::line_wrapper(logger &logger) : logger_(logger) {
}

logger::line_wrapper::~line_wrapper() {
    logger_ << "\n";
}

logger::logger(const utils::string &component) : component_(component) {
}

void logger::set_console(console::console &console) {
    instance_.console_ = &console;
    *instance_.console_ << data_;
}

logger &logger::get_logger() {
    return instance_;
}

logger::line_wrapper logger::operator<<(log_level l) {
    console::console *c = console_ ? console_ : &(default_ = default_printer);
    if (index_ == 0u) {
        *data_ = 0;
    }
    *c << "[" << time::jiffies << "]:[";
    switch (l) {
        case log_level::debug: {
            *c << "DBG";
            break;
        }
        case log_level::info: {
            *c << "INF";
            break;
        }
        case log_level::warning: {
            *c << "WRN";
            break;
        }
        case log_level::error: {
            *c << "ERR";
            break;
        }
        default: {
            *c << "DBG";
            break;
        }
    }
    *c << "]:" << component_ << ": ";

    return line_wrapper(*this);
}

