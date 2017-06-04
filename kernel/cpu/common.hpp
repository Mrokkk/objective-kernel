#pragma once

namespace cpu {

class irq_lock final {

    uint32_t _flags;

public:

    irq_lock() {
        asm volatile("pushfl; popl %0; cli" : "=r" (_flags) :: "memory");
    }

    ~irq_lock() {
        asm volatile("pushl %0; popfl" :: "r" (_flags) : "memory");
    }

};

inline irq_lock make_irq_lock() {
    return {};
}

inline void halt() {
    asm volatile("hlt");
}

inline void cli() {
    asm volatile("cli");
}

inline void sti() {
    asm volatile("sti");
}

} // namespace cpu

