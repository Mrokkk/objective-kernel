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

using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::size_t;

int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);

void *operator new(std::size_t, void *address);
void *operator new[](std::size_t, void *address);

#ifdef __cplusplus
#define asmlinkage extern "C"
#else
#define asmlinkage
#endif

#define PACKED __attribute__((packed))
#define SECTION(x) __attribute__ ((section(#x)))

#define EXPAND(x) x
#define PASTE(a, b) a##b

#define __STRINGIFY(x) #x
#define STRINGIFY(x) __STRINGIFY(x)

namespace yacppl {}

namespace utils = yacppl;

#define __off_t_defined
using off_t = int;

enum errno {
    err_no_error = 0,
    err_no_such_file,
    err_is_a_dir,
    err_exists,
    err_cannot_create,
    err_no_root,
};

#endif

