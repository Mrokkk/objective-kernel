#pragma once

namespace boot {

struct module {
    uint32_t start, end;
    char name[128];
};

extern char bootloader_name[];
extern char cmdline[];
extern uint32_t upper_mem;
extern uint32_t modules_end;
extern module modules[];
extern uint32_t modules_start;
extern uint32_t modules_end;

} // namespace bootloader

