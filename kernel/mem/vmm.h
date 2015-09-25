#ifndef VMM_H
#define VMM_H
#include <stdint.h>
#include "mem/pmm.h"
enum VMM_PTE_FLAGS {
    VMM_PTE_FLAG_PRESENT = 1<<0,
    VMM_PTE_FLAG_WRITABLE = 1<<1,
    VMM_PTE_FLAG_USER = 1<<2,
    // only used for page directory entries
    VMM_PTE_FLAG_WRITETHROUGH = 1<<3,
    VMM_PTE_FLAG_CACHE = 1<<4,
    // flags set by CPU
    VMM_PTE_FLAG_ACCESSED = 1<<5,
    VMM_PTE_FLAG_DIRTY = 1<<6,
    // only used for page directory entries
    VMM_PTE_FLAG_SIZE = 1<<7,
    VMM_PTE_FLAG_GLOBAL = 1<<8,

    VMM_PTE_MASK_FRAME = 0x7FFFF000
};
typedef uint32_t pte_t;
typedef uint32_t pde_t;
typedef uintptr_t vaddr_t;

void init_vmm();
void vmm_test();
void vmm_alloc_page(vaddr_t vaddr, int flags);
void vmm_map_page(vaddr_t vaddr, paddr_t paddr, int flags);
int vmm_get_flags(vaddr_t vaddr);
void vmm_set_flags(vaddr_t vaddr, int flags);
void vmm_unset_flags(vaddr_t vaddr, int flags);
void vmm_unmap_page(vaddr_t vaddr);
void vmm_free_page(vaddr_t vaddr);
void vmm_init_pagetable(vaddr_t vaddr);
void vmm_free_pagetable(vaddr_t vaddr);
void vmm_identity_map(paddr_t addr, int n, int flags);
paddr_t vmm_lookup(vaddr_t addr);
#endif
