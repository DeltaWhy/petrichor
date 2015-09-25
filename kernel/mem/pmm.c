#include <stdbool.h>
#include "globals.h"
#include "mem/pmm.h"
#include "mboot.h"

// 4GB address space / 4KB page size == 1M pages
#define MMAP_LENGTH 1024*1024/32
static uint32_t mmap[MMAP_LENGTH];
#define MMAP_INDEX(addr) ((uint32_t)addr >> 17)
#define MMAP_OFFSET(addr) (((uint32_t)addr >> 12) & 0x1F)

extern const char kernel_start[];
extern const char kernel_end[];

void init_pmm()
{
    // initially mark all memory as unavailable
    for (int i=0; i < MMAP_LENGTH; i++) {
        mmap[i] = 0xFFFFFFFF;
    }

    mboot_mmap_entry *e = boot_info->mmap_addr;
    for (; ((uint32_t)e) < ((uint32_t)boot_info->mmap_addr) + boot_info->mmap_length;
            e = (mboot_mmap_entry *)((uint32_t)e + e->size + 4)) {
        if (e->type != MBOOT_MMAP_TYPE_AVAILABLE) continue;
        // we're 32 bits so we ignore memory above 4GB
        if (e->addr_hi != 0 || e->len_hi != 0) continue;

        if (e->addr_lo == 0) {
            // skip first page, we always want it marked as used
            pmm_free_frames((paddr_t)(e->addr_lo+4096), (e->len_lo >> 12)-1);
        } else {
            pmm_free_frames((paddr_t)e->addr_lo, e->len_lo >> 12);
        }
    }

    // mark first frame used so we only return 0 on error
    mmap[0] |= 0x1;

    // mark kernel memory as reserved so we don't accidentally overwrite it
    pmm_reserve_range((paddr_t)kernel_start, (paddr_t)kernel_end);
    // mark multiboot info as reserved
    pmm_reserve_range((paddr_t)boot_info, ((paddr_t)boot_info)+sizeof(mboot_info));
    pmm_reserve_range((paddr_t)boot_info->mmap_addr, ((paddr_t)boot_info->mmap_addr)+boot_info->mmap_length);
}

paddr_t pmm_alloc_frames(int n)
{
    int i=0;
    int j=0;
    while (i < MMAP_LENGTH) {
        if (mmap[i] == 0xFFFFFFFF) {
            i++;
            j = 0;
            continue;
        }
        // find first free page
        while (mmap[i] & (1<<j) && j < 32) j++;
        if (j == 32) {
            i++;
            j = 0;
            continue;
        }
        // check if n pages are free
        bool found = true;
        for (int k=0; k < n; k++) {
            if (mmap[i] & (1<<j)) {
                // not free, try outer loop again
                found = false;
                break;
            } else {
                j = (j+1)%32;
                if (j == 0) i++;
            }
        }
        if (found) {
            uint32_t addr = (i*32+j)<<12;
            addr -= 4096*n;
            // mark pages as used
            i = MMAP_INDEX(addr);
            j = MMAP_OFFSET(addr);
            for (int k=0; k < n; k++) {
                mmap[i] |= (1<<j);
                j = (j+1)%32;
                if (j == 0) i++;
            }
            return (paddr_t)addr;
        }
    }
    return (paddr_t)0;
}

void pmm_free_frames(paddr_t addr, int n)
{
    if (addr == 0) panic("tried to free null frame");
    int i = MMAP_INDEX(addr);
    int j = MMAP_OFFSET(addr);
    while (n > 0) {
        if (!(mmap[i] & (1<<j))) panic("tried to free frame twice");
        mmap[i] &= ~(1<<j);
        j = (j+1)%32;
        if (j == 0) i++;
        n--;
    }
}

void pmm_reserve_frames(paddr_t addr, int n)
{
    int i = MMAP_INDEX(addr);
    int j = MMAP_OFFSET(addr);
    while (n > 0) {
        mmap[i] |= (1<<j);
        j = (j+1)%32;
        if (j == 0) i++;
        n--;
    }
}

void pmm_reserve_range(paddr_t start, paddr_t end)
{
    pmm_reserve_frames(start, ((end-start)>>12)+1);
}

bool pmm_is_free(paddr_t addr)
{
    return !(mmap[MMAP_INDEX(addr)] & (1<<MMAP_OFFSET(addr)));
}

/* some sanity checks
void test_pmm() {
    uint32_t old_mmap[4];
    paddr_t x;

    memcpy(old_mmap, mmap, 4*sizeof(uint32_t));
    printf("partial mmap: %.8x %.8x %.8x %.8x\n", mmap[0], mmap[1], mmap[2], mmap[3]);
    printf("clearing mmap\n");
    memset(mmap, 0, 4*sizeof(uint32_t));

    printf("allocating 1 page... ");
    x = pmm_alloc_frames(1);
    printf("got %p\n", x);
    printf("partial mmap: %.8x %.8x %.8x %.8x\n", mmap[0], mmap[1], mmap[2], mmap[3]);

    printf("allocating 3 pages... ");
    x = pmm_alloc_frames(3);
    printf("got %p\n", x);
    printf("partial mmap: %.8x %.8x %.8x %.8x\n", mmap[0], mmap[1], mmap[2], mmap[3]);

    printf("freeing first 2 pages\n");
    pmm_free_frames(0x0, 2);
    printf("partial mmap: %.8x %.8x %.8x %.8x\n", mmap[0], mmap[1], mmap[2], mmap[3]);

    printf("allocating 3 pages... ");
    x = pmm_alloc_frames(3);
    printf("got %p\n", x);
    printf("partial mmap: %.8x %.8x %.8x %.8x\n", mmap[0], mmap[1], mmap[2], mmap[3]);

    printf("fancy test\n");
    mmap[0] = 0xffffffff;
    mmap[1] = 0xfff7fff7;
    mmap[2] = 0xffff0fff;
    mmap[3] = 0xffffffff;
    printf("partial mmap: %.8x %.8x %.8x %.8x\n", mmap[0], mmap[1], mmap[2], mmap[3]);
    printf("allocating 2 pages.... ");
    x = pmm_alloc_frames(2);
    printf("got %p\n", x);
    printf("partial mmap: %.8x %.8x %.8x %.8x\n", mmap[0], mmap[1], mmap[2], mmap[3]);

    printf("restoring mmap\n");
    memcpy(mmap, old_mmap, 4*sizeof(uint32_t));
}*/
