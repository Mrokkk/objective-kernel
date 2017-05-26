#pragma once

#include <string.hpp>
#include <kernel/console/console.hpp>

class logger {

    static logger instance_;
    utils::string component_;
    console::console *console_ = nullptr;

    logger(logger &log, const utils::string &component) : component_(component),
            console_(log.console_) {
    }

public:

    logger() = default;

    void initialize(const utils::string &name) {
        component_ = name;
        console_ = instance_.console_;
    }

    static void set_console(console::console &console) {
        instance_.console_ = &console;
    }

    template <typename T>
    logger &operator<<(T t) {
        *console_ << component_ << ": " << t;
        return *this;
    }

};

