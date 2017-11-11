#pragma once

namespace cpu {

struct irq_lock final {

    irq_lock() {
        asm volatile("pushfl; popl %0; cli" : "=r" (_flags) :: "memory");
    }

    ~irq_lock() {
        asm volatile("pushl %0; popfl" :: "r" (_flags) : "memory");
    }

private:
    uint32_t _flags;
};

inline irq_lock make_irq_lock() {
    return irq_lock();
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

