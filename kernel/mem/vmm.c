#include <string.h>
#include "mem/vmm.h"
#include "mem/pmm.h"
#include "sprintf.h"
#include "globals.h"
#include "hw/screen.h"
#include "assert.h"
#include "mboot.h"

#define PD_INDEX(addr) (((addr) >> 22) & 0x3ff)
#define PT_INDEX(addr) (((addr) >> 12) & 0x3ff)
#define MAKE_PDE(addr, flags) ((flags)|((addr)&VMM_PTE_MASK_FRAME))
#define MAKE_PTE(addr, flags) ((flags)|((addr)&VMM_PTE_MASK_FRAME))

extern const char kernel_start[];
extern const char kernel_end[];

static pde_t *pagedir;
void init_vmm()
{
    /* page dirs and page tables have to be page aligned,
     * and they are conveniently also 4KB sized. */
    pagedir = (pde_t *)pmm_alloc_frames(1);
    memset(pagedir, 0, 4096);

    if ((paddr_t)kernel_start > 0x1fffff || (paddr_t)kernel_end > 0x1fffff) panic("kernel is too big or too high in RAM");
    vmm_identity_map((paddr_t)0, 512, VMM_PTE_FLAG_PRESENT | VMM_PTE_FLAG_WRITABLE); // identity map the first two MB of RAM

    // identity map reserved areas - needed for ACPI
    mboot_mmap_entry *e = boot_info->mmap_addr;
    for (; ((uint32_t)e) < ((uint32_t)boot_info->mmap_addr) + boot_info->mmap_length;
            e = (mboot_mmap_entry *)((uint32_t)e + e->size + 4)) {
        if (e->type != MBOOT_MMAP_TYPE_RESERVED && e->type != MBOOT_MMAP_TYPE_ACPI_RECLAIMABLE) continue;
        // we're 32 bits so we ignore memory above 4GB
        if (e->addr_hi != 0 || e->len_hi != 0) continue;
        // skip anything we've already mapped
        if (e->addr_lo < 0x200000) continue;
        vmm_identity_map((paddr_t)e->addr_lo, e->len_lo >> 12, VMM_PTE_FLAG_PRESENT);
    }

    // load the pagedir
    __asm__ __volatile__("mov %0, %%cr3;"
                         "mov %%cr0, %%eax;"
                         "or $0x80000000, %%eax;"
                         "mov %%eax, %%cr0;"
                         : : "b"(pagedir) : "%eax");
}

void vmm_test()
{
    uint32_t *p;
    p = (uint32_t *)0xc0000000;
    assert(!pmm_is_free(0xc0000000));
    vmm_alloc_page(0xc0000000, VMM_PTE_FLAG_PRESENT | VMM_PTE_FLAG_WRITABLE);
    *p = 0xabcd1234;
    vmm_unmap_page(0xc0000000);
    vmm_alloc_page(0xc0000000, VMM_PTE_FLAG_PRESENT | VMM_PTE_FLAG_WRITABLE);
    assert(*p != 0xabcd1234);
}

void vmm_init_pagetable(vaddr_t vaddr)
{
    if (pagedir[PD_INDEX(vaddr)] & VMM_PTE_FLAG_PRESENT) {
        char err[1024];
        sprintf(err, "tried to initialize already initialized pagetable at %p", vaddr);
        panic(err);
    } else {
        paddr_t pt = pmm_alloc_frames(1);
        memset((void *)pt, 0, 4096);
        pagedir[PD_INDEX(vaddr)] = MAKE_PDE(pt, VMM_PTE_FLAG_PRESENT | VMM_PTE_FLAG_WRITABLE |
                                            VMM_PTE_FLAG_CACHE);
    }
}

void vmm_free_pagetable(vaddr_t vaddr)
{
    if (!(pagedir[PD_INDEX(vaddr)] & VMM_PTE_FLAG_PRESENT)) {
        char err[1024];
        sprintf(err, "tried to free nonexistent pagetable at %p", vaddr);
        panic(err);
    } else {
        paddr_t pt = pagedir[PD_INDEX(vaddr)] & VMM_PTE_MASK_FRAME;
        pmm_free_frames(pt, 1);
        pagedir[PD_INDEX(vaddr)] &= ~(VMM_PTE_FLAG_PRESENT);
    }
}

void vmm_alloc_page(vaddr_t vaddr, int flags)
{
    // TODO - check for null?
    vmm_map_page(vaddr, pmm_alloc_frames(1), flags);
}

void vmm_map_page(vaddr_t vaddr, paddr_t paddr, int flags)
{
    if (!(pagedir[PD_INDEX(vaddr)] & VMM_PTE_FLAG_PRESENT)) {
        vmm_init_pagetable(vaddr);
    }
    //TODO - don't assume the pagetable is identity mapped
    pte_t *pt = (pte_t *)(pagedir[PD_INDEX(vaddr)] & VMM_PTE_MASK_FRAME);
    if (pt[PT_INDEX(vaddr)] & VMM_PTE_FLAG_PRESENT) {
        char err[1024];
        sprintf(err, "tried to map already existing page at %p", vaddr);
        panic(err);
    } else {
        pt[PT_INDEX(vaddr)] = MAKE_PTE(paddr, flags);
    }
}

int vmm_get_flags(vaddr_t vaddr)
{
    if (!(pagedir[PD_INDEX(vaddr)] & VMM_PTE_FLAG_PRESENT)) {
        return 0;
    }
    pte_t *pt = (pte_t *)(pagedir[PD_INDEX(vaddr)] & VMM_PTE_MASK_FRAME);
    if (!(pt[PT_INDEX(vaddr)] & VMM_PTE_FLAG_PRESENT)) {
        return 0;
    }
    return pt[PT_INDEX(vaddr)] & ~(VMM_PTE_MASK_FRAME);
}

void vmm_set_flags(vaddr_t vaddr, int flags)
{
    if (!(pagedir[PD_INDEX(vaddr)] & VMM_PTE_FLAG_PRESENT)) {
        char err[1024];
        sprintf(err, "tried to set page flags in nonexistent pagetable at %p", vaddr);
        panic(err);
    }
    pte_t *pt = (pte_t *)(pagedir[PD_INDEX(vaddr)] & VMM_PTE_MASK_FRAME);
    if (!(pt[PT_INDEX(vaddr)] & VMM_PTE_FLAG_PRESENT)) {
        char err[1024];
        sprintf(err, "tried to set flags on invalid page at %p", vaddr);
        panic(err);
    } else {
        pt[PT_INDEX(vaddr)] |= flags;
        __asm__ __volatile__ ("invlpg (%0)": : "a"(vaddr));
    }
}

void vmm_unset_flags(vaddr_t vaddr, int flags)
{
    if (!(pagedir[PD_INDEX(vaddr)] & VMM_PTE_FLAG_PRESENT)) {
        char err[1024];
        sprintf(err, "tried to set page flags in nonexistent pagetable at %p", vaddr);
        panic(err);
    }
    pte_t *pt = (pte_t *)(pagedir[PD_INDEX(vaddr)] & VMM_PTE_MASK_FRAME);
    if (!(pt[PT_INDEX(vaddr)] & VMM_PTE_FLAG_PRESENT)) {
        char err[1024];
        sprintf(err, "tried to set flags on invalid page at %p", vaddr);
        panic(err);
    } else {
        pt[PT_INDEX(vaddr)] &= ~(flags);
        __asm__ __volatile__ ("invlpg (%0)": : "a"(vaddr));
    }
}

void vmm_unmap_page(vaddr_t vaddr)
{
    if (!(pagedir[PD_INDEX(vaddr)] & VMM_PTE_FLAG_PRESENT)) {
        char err[1024];
        sprintf(err, "tried to free page in nonexistent pagetable at %p", vaddr);
        panic(err);
    }
    pte_t *pt = (pte_t *)(pagedir[PD_INDEX(vaddr)] & VMM_PTE_MASK_FRAME);
    if (!(pt[PT_INDEX(vaddr)] & VMM_PTE_FLAG_PRESENT)) {
        char err[1024];
        sprintf(err, "tried to free already free page at %p", vaddr);
        panic(err);
    } else {
        pt[PT_INDEX(vaddr)] &= ~(VMM_PTE_FLAG_PRESENT);
        __asm__ __volatile__ ("invlpg (%0)": : "a"(vaddr));
    }
}

void vmm_free_page(vaddr_t vaddr)
{
    paddr_t paddr = vmm_lookup(vaddr);
    vmm_unmap_page(vaddr);
    if (!pmm_is_free(paddr)) pmm_free_frames(paddr, 1);
}

void vmm_identity_map(paddr_t addr, int n, int flags)
{
    for (int i=0; i < n; i++) {
        paddr_t page = addr+0x1000*i;
        vmm_map_page((vaddr_t)page, page, flags);
    }
}

paddr_t vmm_lookup(vaddr_t addr)
{
    if (!(pagedir[PD_INDEX(addr)] & VMM_PTE_FLAG_PRESENT)) return (paddr_t)0;
    pte_t *pt = (pte_t *)(pagedir[PD_INDEX(addr)] & VMM_PTE_MASK_FRAME);
    if (!(pt[PT_INDEX(addr)] & VMM_PTE_FLAG_PRESENT)) return (paddr_t)0;
    paddr_t frame = pt[PT_INDEX(addr)] & VMM_PTE_MASK_FRAME;
    return frame | (addr & 0xfff);
}
