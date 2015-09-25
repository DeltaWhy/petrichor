#ifndef ELF_H
#define ELF_H
#include <stdint.h>

#define ELF_SEG_TYPE_NULL 0
#define ELF_SEG_TYPE_LOAD 1
#define ELF_SEG_TYPE_DYNAMIC 2
#define ELF_SEG_TYPE_INTERP 3
#define ELF_SEG_TYPE_NOTE 4

#define ELF_SEG_FLAG_EXECUTABLE 1
#define ELF_SEG_FLAG_WRITABLE 2
#define ELF_SEG_FLAG_READABLE 4

typedef struct elf_prog_header {
    uint32_t type;
    uint32_t offset;
    void *vaddr;
    uint32_t undef;
    uint32_t filesize;
    uint32_t memsize;
    uint32_t flags;
    uint32_t alignment;
} elf_prog_header;

#define ELF_SECT_TYPE_NULL 0
#define ELF_SECT_TYPE_PROGBITS 1
#define ELF_SECT_TYPE_SYMTAB 2
#define ELF_SECT_TYPE_STRTAB 3
#define ELF_SECT_TYPE_RELA 4
#define ELF_SECT_TYPE_HASH 5
#define ELF_SECT_TYPE_DYNAMIC 6
#define ELF_SECT_TYPE_NOTE 7
#define ELF_SECT_TYPE_NOBITS 8
#define ELF_SECT_TYPE_REL 9
#define ELF_SECT_TYPE_SHLIB 10
#define ELF_SECT_TYPE_DYNSYM 11

#define ELF_SECT_FLAG_WRITE 1
#define ELF_SECT_FLAG_ALLOC 2
#define ELF_SECT_FLAG_EXECINSTR 4

typedef struct elf_sect_header {
    uint32_t name;
    uint32_t type;
    uint32_t flags;
    void *addr;
    uint32_t offset;
    uint32_t size;
    uint32_t link;
    uint32_t info;
    uint32_t align;
    uint32_t entrysize;
} elf_sect_header;

typedef struct elf_sym {
    uint32_t name;
    uint32_t value;
    uint32_t size;
    uint8_t info;
    uint8_t other;
    uint16_t shndx;
} elf_sym;

#define ELF_MAGIC 0x464c457f

#define ELF_WORDSIZE_32 1
#define ELF_WORDSIZE_64 2

#define ELF_LITTLE_ENDIAN 1
#define ELF_BIG_ENDIAN 2

#define ELF_TYPE_RELOCATABLE 1
#define ELF_TYPE_EXECUTABLE 2
#define ELF_TYPE_SHARED 3
#define ELF_TYPE_CORE 4

#define ELF_INSTRUCTION_SET_X86 0x3
#define ELF_INSTRUCTION_SET_X86_64 0x3E
#define ELF_INSTRUCTION_SET_ARM 0x28

typedef struct elf_header {
    uint32_t magic;
    uint8_t wordsize;
    uint8_t endianness;
    uint8_t version;
    uint8_t abi;
    uint8_t pad[8];
    uint16_t type;
    uint16_t instruction_set;
    uint32_t version2;
    void *entry;
    uint32_t prog_table_offset;
    uint32_t sect_table_offset;
    uint32_t flags;
    uint16_t header_size;
    uint16_t prog_table_entry_size;
    uint16_t prog_table_numentries;
    uint16_t sect_table_entry_size;
    uint16_t sect_table_numentries;
    uint16_t sect_names_index;
} elf_header;

#endif
