#pragma once

#ifdef __ASSEMBLER__

#define ENTRY(name) \
    .global name; \
    name:

#define ALIGN(x) \
    .align x

#define SECTION(sec) \
    .section sec

#else

#include <cstddef>
#include <cstdint>
#include <cstdarg>

int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);

#ifdef __cplusplus
#define asmlinkage extern "C"
#else
#define asmlinkage
#endif

#define __packed __attribute__((packed))
#define __noreturn __attribute__((noreturn))
#define __naked __attribute__ ((naked))
#define __section(x) __attribute__ ((section(#x)))
#define __optimize(x) __attribute__ ((optimize(#x)))

#define expand(x) x
#define paste(a, b) a##b

#define __stringify(x) #x
#define stringify(x) __stringify(x)

namespace yacppl {}

namespace utils = yacppl;

using off_t = size_t;

#endif
