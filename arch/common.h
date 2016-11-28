#pragma once

namespace cpu {

inline void mb() {
    asm volatile("" ::: "memory");
}

inline void halt() {
    asm volatile("hlt" ::: "memory");
}

namespace detail {

class irq_lock {

    uint32_t _flags;

public:

    irq_lock() {
        asm volatile("pushfl; popl %0; cli" : "=r" (_flags) :: "memory");
    }

    ~irq_lock() {
        asm volatile("pushl %0; popfl" :: "r" (_flags) : "memory");
    }

};

} // namespace detail

inline detail::irq_lock irq_save() {
    return {};
}

} // namespace cpu

