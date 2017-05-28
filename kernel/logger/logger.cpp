#include <kernel/time/time.hpp>
#include "logger.hpp"

logger logger::instance_("");
console::console *logger::console_;
utils::spinlock logger::spinlock_;

logger::line_wrapper::line_wrapper(logger &logger) : logger_(logger) {
}

logger::line_wrapper::~line_wrapper() {
    logger_ << "\n";
}

logger::logger(const utils::string &component) : component_(component) {
}

void logger::set_console(console::console &console) {
    instance_.console_ = &console;
}

logger &logger::get_logger() {
    return instance_;
}

logger::line_wrapper logger::operator<<(log_level l) {
    if (console_) {
        *console_ << "[" << time::jiffies << "]:[";
        switch (l) {
            case log_level::debug: {
                *console_ << "DBG";
                break;
            }
            case log_level::info: {
                *console_ << "INF";
                break;
            }
            case log_level::warning: {
                *console_ << "WRN";
                break;
            }
            case log_level::error: {
                *console_ << "ERR";
                break;
            }
            default: {
                *console_ << "DBG";
                break;
            }
        }
        *console_ << "]:" << component_ << ": ";
    }
    return line_wrapper(*this);
}

