#pragma once

namespace cpu {

struct irq_lock final {

    irq_lock() {
        asm volatile("pushfl; popl %0; cli" : "=r" (flags_) :: "memory");
    }

    ~irq_lock() {
        asm volatile("pushl %0; popfl" :: "r" (flags_) : "memory");
    }

private:
    uint32_t flags_;
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

