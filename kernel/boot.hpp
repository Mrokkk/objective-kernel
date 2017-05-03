#pragma once

namespace boot {

extern char cmdline[];
extern uint32_t upper_mem;

char *read_cmdline(void *data, uint32_t magic);

} // namespace bootloader


