#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
#define asmlinkage extern "C"
#else
#define asmlinkage
#endif

#define __packed __attribute__((packed))
#define __noreturn __attribute__((noreturn))

