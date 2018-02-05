#pragma once

#include <string.hpp>
#include <spinlock.hpp>
#include <kernel/device/character.hpp>

struct logger {

    struct line_wrapper {
        template <typename T>
        line_wrapper& operator<<(T t) {
            logger_ << t;
            return *this;
        }
        ~line_wrapper();
        friend logger;
    private:
        explicit line_wrapper(logger& logger);
        logger& logger_;
    };

    enum log_level {
        debug, info, warning, error
    };

    logger(const char* component);

    static void set_driver(device::character* device);
    line_wrapper operator<<(const log_level l);

    static int printf(const char* fmt, ...) {
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsprintf(buf, fmt, args);
        va_end(args);
        print(buf);
        return 0;
    }

    friend line_wrapper;

private:

    static void print(const char* c) {
        const auto len = utils::length(c);
        if (device_) {
            device_->write(c, len);
        }
        else {
            utils::copy(c, data_ + index_);
            index_ += len;
        }
    }

    logger& operator<<(const char* str);
    logger& operator<<(char str[]);
    logger& operator<<(int a);
    logger& operator<<(uint64_t a);
    logger& operator<<(uint32_t a);
    logger& operator<<(uint16_t a);
    logger& operator<<(uint8_t a);
    logger& operator<<(char c);

    template <typename T>
    typename utils::enable_if<
        !utils::is_same<
            typename utils::remove_const<T>::type, char
        >::value, logger &
    >::type operator<<(T* a) {
        char buf[32];
        sprintf(buf, "0x%08x", reinterpret_cast<uint32_t>(a));
        print(buf);
        return *this;
    }

    static logger instance_;
    static utils::spinlock spinlock_;
    static device::character* device_;
    static char data_[4096];
    static size_t index_;
    const char* component_;
};

