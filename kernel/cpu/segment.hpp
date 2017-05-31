#pragma once

#define KERNEL_CS 0x08
#define KERNEL_DS 0x10
#define USER_CS 0x1b
#define USER_DS 0x23
#define FIRST_TSS 0x28

#ifndef __ASSEMBLER__

namespace cpu {

enum segment {
    kernel_cs = KERNEL_CS,
    kernel_ds = KERNEL_DS,
    user_cs = USER_CS,
    user_ds = USER_DS,
    task = FIRST_TSS
};

} // namespace cpu

#endif

