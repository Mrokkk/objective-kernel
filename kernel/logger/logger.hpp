#pragma once

#include <string.hpp>
#include <spinlock.hpp>

struct logger {

    using printer_function = void(*)(const char *);

    enum class log_level {
        debug, info, warning, error
    };

private:

    static logger instance_;
    static utils::spinlock spinlock_;
    static printer_function printer_;
    static char data_[4096];
    static size_t index_;
    const char *component_;

    static void default_printer(const char *c) {
        utils::copy(c, data_ + index_);
        index_ += utils::length(c);
    }

    logger &operator=(printer_function fn);
    logger &operator<<(const char *str);
    logger &operator<<(char str[]);
    logger &operator<<(int a);
    logger &operator<<(uint64_t a);
    logger &operator<<(uint32_t a);
    logger &operator<<(uint16_t a);
    logger &operator<<(uint8_t a);
    logger &operator<<(char c);

    template <typename T>
    typename utils::enable_if<
        !utils::is_same<
            typename utils::remove_const<T>::type, char
        >::value, logger &
    >::type operator<<(T *a) {
        char buf[32];
        sprintf(buf, "0x%08x", reinterpret_cast<uint32_t>(a));
        printer_(buf);
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

    logger(const char *component);

    static void set_printer_function(printer_function fn);
    line_wrapper operator<<(log_level l);

    friend line_wrapper;

};

