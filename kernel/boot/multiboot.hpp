#pragma once

// Multiboot1
#define MULTIBOOT_HEADER_MAGIC          0x1BADB002
#define MULTIBOOT_HEADER_FLAGS_ALIGNED  (1 << 0)
#define MULTIBOOT_HEADER_FLAGS_MEMORY   (1 << 1)
#define MULTIBOOT_HEADER_FLAGS_VIDEO    (1 << 2)
#define MULTIBOOT_HEADER_FLAGS_ADDRESS  (1 << 16)
#define MULTIBOOT_HEADER_FLAGS \
    MULTIBOOT_HEADER_FLAGS_ALIGNED | MULTIBOOT_HEADER_FLAGS_MEMORY

#define MULTIBOOT_HEADER \
    .long MULTIBOOT_HEADER_MAGIC; \
    .long MULTIBOOT_HEADER_FLAGS; \
    .long -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS);

// Multiboot2
#define MULTIBOOT2_HEADER_MAGIC 0xE85250D6
#define MULTIBOOT2_HEADER_ARCH  0

#define MULTIBOOT2_HEADER \
    header_start: \
    .long MULTIBOOT2_HEADER_MAGIC; \
    .long MULTIBOOT2_HEADER_ARCH; \
    .long header_end - header_start; \
    .long -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT2_HEADER_ARCH + header_end - header_start); \
    .word 1; \
    .word 0; \
    .long 4+8; \
    .long 1; \
    .balign 8; \
    .word 0; \
    .word 0; \
    .long 8; \
    header_end:

#define MULTIBOOT_BOOTLOADER_MAGIC  0x2BADB002
#define MULTIBOOT2_BOOTLOADER_MAGIC 0x36d76289

#ifndef __ASSEMBLER__

/* The Multiboot header. */
struct multiboot_header {
    unsigned long magic;
    unsigned long flags;
    unsigned long checksum;
    unsigned long header_addr;
    unsigned long load_addr;
    unsigned long load_end_addr;
    unsigned long bss_end_addr;
    unsigned long entry_addr;
};

/* The symbol table for a.out. */
struct aout_symbol_table {
    unsigned long tabsize;
    unsigned long strsize;
    unsigned long addr;
    unsigned long reserved;
};

/* The section header table for ELF. */
struct elf_section_header_table {
    unsigned long num;
    unsigned long size;
    unsigned long addr;
    unsigned long shndx;
};

/* The Multiboot information. */
struct multiboot_info {
    unsigned long flags;
    unsigned long mem_lower;
    unsigned long mem_upper;
    unsigned long boot_device;
    char *cmdline;
    unsigned long mods_count;
    unsigned long mods_addr;
    union {
        struct aout_symbol_table aout_sym;
        struct elf_section_header_table elf_sec;
    };
    unsigned long mmap_length;
    unsigned long mmap_addr;
    unsigned long drives_length;
    unsigned long drives_addr;
    unsigned long config_table;
    char *bootloader_name;
    unsigned long apm_table;
    struct vbe_struct {
        unsigned long control_info;
        unsigned long mode_info;
        unsigned long mode;
        unsigned long interface_seg;
        unsigned long interface_off;
        unsigned long interface_len;
    } vbe;
} __packed;

/* The module structure. */
struct module {
    unsigned long mod_start;
    unsigned long mod_end;
    unsigned long string;
    unsigned long reserved;
};

/* The memory map. Be careful that the offset 0 is base_addr_low
but no size. */
struct memory_map {
    unsigned long size;
    unsigned long base_addr_low;
    unsigned long base_addr_high;
    unsigned long length_low;
    unsigned long length_high;
    unsigned long type;
};

struct multiboot_drives_struct {
    unsigned int size;
    unsigned char drive_number;
    unsigned char drive_mode;
    unsigned short drive_cylinders;
    unsigned char drive_heads;
    unsigned char drive_sectors;
    unsigned char drive_ports;
};

struct multiboot_apm_table_struct {
    unsigned short version;
    unsigned short cseg;
    unsigned int offset;
    unsigned short cseg_16;
    unsigned short dseg;
    unsigned short flags;
    unsigned short cseg_len;
    unsigned short cseg_16_len;
    unsigned short dseg_len;
};

#define MULTIBOOT_FLAGS_MEM_BIT             (1 << 0)
#define MULTIBOOT_FLAGS_BOOTDEV_BIT         (1 << 1)
#define MULTIBOOT_FLAGS_CMDLINE_BIT         (1 << 2)
#define MULTIBOOT_FLAGS_MODS_BIT            (1 << 3)
#define MULTIBOOT_FLAGS_SYMS_BIT            ((1 << 4) | (1 << 5))
#define MULTIBOOT_FLAGS_MMAP_BIT            (1 << 6)
#define MULTIBOOT_FLAGS_DRIVES_BIT          (1 << 7)
#define MULTIBOOT_FLAGS_CONFIG_TABLE_BIT    (1 << 8)
#define MULTIBOOT_FLAGS_BL_NAME_BIT         (1 << 9)
#define MULTIBOOT_FLAGS_APM_TABLE_BIT       (1 << 10)
#define MULTIBOOT_FLAGS_VBE_BIT             (1 << 11)

struct mb2_tags_header {
    uint32_t size;
    uint32_t reserved;
} __packed;

struct mb2_tag {
    uint32_t type;
    uint32_t size;
} __packed;


#define MULTIBOOT_TAG_TYPE_END               0
#define MULTIBOOT_TAG_TYPE_CMDLINE           1
#define MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME  2
#define MULTIBOOT_TAG_TYPE_MODULE            3
#define MULTIBOOT_TAG_TYPE_BASIC_MEMINFO     4
#define MULTIBOOT_TAG_TYPE_BOOTDEV           5
#define MULTIBOOT_TAG_TYPE_MMAP              6
#define MULTIBOOT_TAG_TYPE_VBE               7
#define MULTIBOOT_TAG_TYPE_FRAMEBUFFER       8
#define MULTIBOOT_TAG_TYPE_ELF_SECTIONS      9
#define MULTIBOOT_TAG_TYPE_APM               10

#endif /* __ASSEMBLER__ */

