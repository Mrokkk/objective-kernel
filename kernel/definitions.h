#pragma once

#ifdef __ASSEMBLER__

#define ENTRY(name) \
    .global name; \
    name:

#else

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
#define asmlinkage extern "C"
#else
#define asmlinkage
#endif

#define __packed __attribute__((packed))
#define __noreturn __attribute__((noreturn))

#endif

