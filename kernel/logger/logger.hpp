#pragma once

#include <string.hpp>
#include <kernel/console/console.hpp>


struct logger {

    enum class log_level {
        debug, info, warning, error
    };

private:

    static logger instance_;
    utils::string component_;
    console::console *console_ = nullptr;

    logger(logger &log, const utils::string &component) : component_(component),
            console_(log.console_) {
    }

    template <typename T>
    logger &operator<<(T t) {
        if (console_) {
            *console_ << t;
        }
        return *this;
    }

    static const char *log_level_to_string(log_level l) {
        switch (l) {
            case log_level::debug: {
                return "DBG";
            }
            case log_level::warning: {
                return "WRN";
            }
            default: {
                return "DBG";
            }
        }
    }

    class line_wrapper {

        logger &logger_;

    public:

        explicit line_wrapper(logger &logger, log_level) : logger_(logger) {
            logger_ << "[" << logger_.component_ << "]:DEBUG: ";
        }

        template <typename T>
        line_wrapper &operator<<(T t) {
            logger_ << t;
            return *this;
        }

        ~line_wrapper() {
            logger_ << "\n";
        }

    };

public:

    logger() = default;

    void initialize(const utils::string &name) {
        component_ = name;
        console_ = instance_.console_;
    }

    static void set_console(console::console &console) {
        instance_.console_ = &console;
    }

    line_wrapper operator<<(log_level l) {
        return line_wrapper(*this, l);
    }

    friend line_wrapper;

};

