#pragma once

namespace boot {

extern char cmdline[];
char *read_cmdline(void *data, uint32_t magic);

} // namespace bootloader


