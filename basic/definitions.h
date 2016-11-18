#pragma once

#include <stddef.h>

#ifdef __cplusplus
#define asmlinkage extern "C"
#else
#define asmlinkage
#endif

#define __packed __attribute__((packed))
#define __noreturn __attribute__((noreturn))

