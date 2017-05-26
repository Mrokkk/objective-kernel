#pragma once

#include <string.hpp>
#include <kernel/console/console.hpp>

struct logger {

    enum class log_level {
        debug, info, warning, error
    };

private:

    static logger instance_;
    static utils::spinlock spinlock_;
    utils::string component_;
    console::console *console_ = nullptr;

    logger(logger &log, const utils::string &component);

    template <typename T>
    logger &operator<<(T t) {
        utils::scoped_lock l(spinlock_);
        if (console_) {
            *console_ << t;
        }
        return *this;
    }

    class line_wrapper {

        logger &logger_;

    public:

        explicit line_wrapper(logger &logger);
        ~line_wrapper();

        template <typename T>
        line_wrapper &operator<<(T t) {
            logger_ << t;
            return *this;
        }

    };

public:

    logger() = default;
    void initialize(const utils::string &name);
    static void set_console(console::console &console);
    line_wrapper operator<<(log_level l);

    friend line_wrapper;

};

