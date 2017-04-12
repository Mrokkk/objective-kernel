#include <string.h>
#include <kernel/multiboot.hpp>
#include <kernel/console/console.hpp>

asmlinkage {

extern void (*__init_array_start)();
extern void (*__init_array_end)();
extern void (*__preinit_array_start)();
extern void (*__preinit_array_end)();

void * __dso_handle = 0;

int __cxa_atexit(void (*)(void *), void *, void *) {
    return 0;
}

void __cxa_pure_virtual() {
    console::print("PANIC: called pure virtual method\n");
    while (1);
}

char cmdline[128];

// for clang
void *memset(void *ptr, int value, unsigned n) {
    for (auto i = 0u; i < n; ++i) {
        *reinterpret_cast<char *>(ptr) = value;
    }
    return ptr;
}

void _init(void *data, uint32_t magic) {
    // TODO: move to other file
    auto cmdline_ptr = bootloader::read_cmdline(data, magic);
    if (cmdline_ptr == nullptr) {
        cmdline[0] = 0;
    }
    else {
        utils::copy(cmdline_ptr, cmdline);
    }
    void (**preinit_constructor)() = &__preinit_array_start;
    void (**init_constructor)() = &__init_array_start;
    while (preinit_constructor != &__preinit_array_end) {
        (*preinit_constructor)();
        ++preinit_constructor;
    }
    while (init_constructor != &__init_array_end) {
        (*init_constructor)();
        ++init_constructor;
    }
}

}

