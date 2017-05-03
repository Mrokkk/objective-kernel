#pragma once

inline uint32_t cr0_get() {
    uint32_t rv;
    asm volatile("mov %%cr0, %0" : "=r" (rv));
    return rv;
}

inline uint32_t cr1_get() {
    uint32_t rv;
    asm volatile("mov %%cr1, %0" : "=r" (rv));
    return rv;
}

inline uint32_t cr2_get() {
    uint32_t rv;
    asm volatile("mov %%cr2, %0" : "=r" (rv));
    return rv;
}

