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
    static console::console *console_;

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

    logger(const utils::string &component);

    static void set_console(console::console &console);
    static logger &get_logger();
    line_wrapper operator<<(log_level l);

    friend line_wrapper;

};

#define ASSERT(cond) \
    do { \
        if (!(cond)) { \
            ::logger::get_logger() << ::logger::log_level::error << (utils::last_occurrence(__FILE__, '/') + 1) << ":" << __LINE__ << ": assertion failed: " << #cond << "\n"; \
        } \
    } while (0)

