#ifndef MBOOT_H
#define MBOOT_H

#include <stdint.h>
enum MBOOT_MMAP_TYPE {
    MBOOT_MMAP_TYPE_AVAILABLE = 1,
    MBOOT_MMAP_TYPE_RESERVED = 2,
    MBOOT_MMAP_TYPE_ACPI_RECLAIMABLE = 3,
    MBOOT_MMAP_TYPE_NVS = 4,
    MBOOT_MMAP_TYPE_BAD = 5
};

typedef struct mboot_mmap_entry {
    uint32_t size;
    uint32_t addr_lo;
    uint32_t addr_hi;
    uint32_t len_lo;
    uint32_t len_hi;
    uint32_t type;
} mboot_mmap_entry;

enum MBOOT_FLAGS {
    MBOOT_FLAG_MEMORY = 0x1,
    MBOOT_FLAG_BOOTDEV = 0x2,
    MBOOT_FLAG_CMDLINE = 0x4,
    MBOOT_FLAG_MODS = 0x8,
    MBOOT_FLAG_AOUT_SYMS = 0x10,
    MBOOT_FLAG_ELF_SHDR = 0x20,
    MBOOT_FLAG_MEM_MAP = 0x40,
    MBOOT_FLAG_DRIVE_INFO = 0x80,
    MBOOT_FLAG_CONFIG_TABLE = 0x100,
    MBOOT_FLAG_BOOTLOADER_NAME = 0x200,
    MBOOT_FLAG_APM_TABLE = 0x400,
    MBOOT_FLAG_VBE_INFO = 0x800
};

typedef struct mboot_module {
    void *mod_start;
    void *mod_end;
    char cmdline[0];
    uint32_t pad;
} mboot_module;

typedef struct mboot_info {
    uint32_t flags;
    uint32_t mem_lo;
    uint32_t mem_hi;
    uint32_t boot_device;
    char *cmd_line;
    uint32_t mods_count;
    mboot_module *mods_addr;
    uint32_t elf_section_num;
    uint32_t elf_section_size;
    void *elf_section_addr;
    uint32_t elf_section_shndx;
    uint32_t mmap_length;
    mboot_mmap_entry *mmap_addr;
    uint32_t drives_length;
    void *drives_addr;
    uint32_t config_table;
    char *bootloader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
} mboot_info;

extern mboot_info *boot_info;
#endif
