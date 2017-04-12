#include "multiboot.hpp"

char *multiboot_read(struct multiboot_info *mb) {

    return (char *)mb->cmdline;

}

