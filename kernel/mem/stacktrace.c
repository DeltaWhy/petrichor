#include "assert.h"
#include <stdint.h>
#include "mboot.h"
#include "globals.h"
#include "sprintf.h"
#include <string.h>
#include "mem/malloc.h"
#include "mem/stacktrace.h"
#include "hw/screen.h"
#include "hw/port.h"
#include "task/elf.h"
extern void *stack_top;
extern void *stack_bottom;

typedef struct symbol {
    void *addr;
    char *name;
} symbol;
size_t num_symbols;
symbol *symbols = NULL;

void init_stacktrace(void)
{
    if (!(boot_info->flags & MBOOT_FLAG_ELF_SHDR)) return;
    assert(boot_info->elf_section_size == sizeof(elf_sect_header));
    elf_sect_header *sect_table = boot_info->elf_section_addr;
    elf_sect_header *shstr = sect_table + boot_info->elf_section_shndx;

    elf_sect_header *symtab;
    elf_sect_header *strtab;
    for (unsigned int i = 0; i < boot_info->elf_section_num; i++) {
        if (strcmp(shstr->addr+sect_table[i].name, ".symtab") == 0) symtab=sect_table+i;
        if (strcmp(shstr->addr+sect_table[i].name, ".strtab") == 0) strtab=sect_table+i;
    }
    assert(symtab != NULL);
    assert(strtab != NULL);
    assert(symtab->size != 0);
    assert(symtab->entrysize != 0);

    elf_sym *syms = symtab->addr;
    char *strs = strtab->addr;

    num_symbols = symtab->size/symtab->entrysize;
    symbols = calloc(num_symbols, sizeof(symbol));
    assert(symbols != NULL);

    for (unsigned int i=0; i < num_symbols; i++) {
        // insert sorted by descending memory address
        unsigned int j=0;
        while (j < i && symbols[j].addr >= (void *)syms[i].value) j++;
        if (j < i) {
            for (unsigned int k = i; k > j; k--) symbols[k] = symbols[k-1];
        }
        symbols[j].addr = (void *)syms[i].value;
        symbols[j].name = strs+syms[i].name;
    }
}

void stacktrace(void)
{
    void **ebp;
    __asm__ ("movl %%ebp, %0"
             : "=a"(ebp));
    printf("call stack:\n");
    bochslog("call stack:\naddr2line -fspi -e kernel.elf ");
    while (ebp < &stack_top && ebp >= &stack_bottom) {
        char str[11];
        void *raddr = ebp[1];
        sprintf(str, "%p", raddr);
        printf("    %p", raddr);
        if (symbols != NULL) {
            symbol *symp = symbols;
            while (symp->addr > raddr || strlen(symp->name) == 0) symp++;
            printf(" (%s+%#x)\n", symp->name, ((uintptr_t)raddr)-((uintptr_t)symp->addr));
        } else {
            print("\n");
        }
        bochslog(str);
        bochslog(" ");
        ebp = ebp[0];
    }
    bochslog("\n");
}

void stacktrace_from(void *eip, void **ebp)
{
    printf("call stack:\n");
    bochslog("call stack:\naddr2line -fspi -e kernel.elf ");
    char str[11];
    void *raddr = eip;
    sprintf(str, "%p", raddr);
    symbol *symp = symbols;
    while (symp->addr > raddr || strlen(symp->name) == 0) symp++;
    printf("    %p (%s+%#x)\n", raddr, symp->name, ((uintptr_t)raddr)-((uintptr_t)symp->addr));
    bochslog(str);
    bochslog(" ");
    while (ebp < &stack_top && ebp >= &stack_bottom) {
        char str[11];
        void *raddr = ebp[1];
        sprintf(str, "%p", raddr);
        symbol *symp = symbols;
        while (symp->addr > raddr || strlen(symp->name) == 0) symp++;
        printf("    %p (%s+%#x)\n", raddr, symp->name, ((uintptr_t)raddr)-((uintptr_t)symp->addr));
        bochslog(str);
        bochslog(" ");
        ebp = ebp[0];
    }
    bochslog("\n");
}
