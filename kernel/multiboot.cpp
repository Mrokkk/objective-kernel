#include "multiboot.hpp"

#define virt_address(phys) \
    ((char *)phys + 0xc0100000)

char *multiboot_read(struct multiboot_info *mb) {

    return (char *)mb->cmdline ;

}

