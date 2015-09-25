#include "globals.h"
#include "mem/vmm.h"
#include "mem/sbrk.h"

char *kernel_heap_start = (char *)0x200000;
char *kernel_heap_end = (char *)0x200000;

/*
 * @return 0 on success, -1 on error
 */
int brk(void *addr)
{
    if ((char *)addr < kernel_heap_start) return -1;
    // round heap end to next free page
    kernel_heap_end = (char *)(((uintptr_t)kernel_heap_end + 0xfff)&~(0xfff));

    if ((char *)addr > kernel_heap_end) {
        int npages = ((char *)addr+0xfff - kernel_heap_end)>>12;
        for (int i=0; i < npages; i++) {
            vmm_alloc_page((vaddr_t)kernel_heap_end, VMM_PTE_FLAG_PRESENT | VMM_PTE_FLAG_WRITABLE);
            kernel_heap_end += 0x1000;
        }
        kernel_heap_end = addr;
        return 0;
    } else if ((char *)addr <= kernel_heap_end-0x1000) {
        kernel_heap_end -= 0x1000;
        while (kernel_heap_end >= (char *)addr) {
            vmm_free_page((vaddr_t)kernel_heap_end);
            kernel_heap_end -= 0x1000;
        }
        kernel_heap_end = addr;
        return 0;
    } else {
        kernel_heap_end = addr;
        return 0;
    }
}

/*
 * @return previous break on success, -1 on error
 */
void *sbrk(intptr_t increment)
{
    void *prev = kernel_heap_end;
    if (brk(kernel_heap_end+increment) != 0) return (void *)-1;
    return prev;
}

/*
void test_sbrk() {
    assert(kernel_heap_start == kernel_heap_end);
    assert(!(vmm_get_flags((vaddr_t)kernel_heap_start) & VMM_PTE_FLAG_PRESENT));

    // test incrementing break
    assert(brk((void *)0x200001) == 0);
    assert(kernel_heap_end == (void *)0x200001);
    assert(vmm_get_flags((vaddr_t)kernel_heap_start) & VMM_PTE_FLAG_PRESENT);
    assert(!(vmm_get_flags((vaddr_t)kernel_heap_start+0x1000) & VMM_PTE_FLAG_PRESENT));

    assert(brk((void *)0x200fff) == 0);
    assert(kernel_heap_end == (void *)0x200fff);
    assert(vmm_get_flags((vaddr_t)kernel_heap_start) & VMM_PTE_FLAG_PRESENT);
    assert(!(vmm_get_flags((vaddr_t)kernel_heap_start+0x1000) & VMM_PTE_FLAG_PRESENT));

    assert(brk((void *)0x201fff) == 0);
    assert(kernel_heap_end == (void *)0x201fff);
    assert(vmm_get_flags((vaddr_t)kernel_heap_start) & VMM_PTE_FLAG_PRESENT);
    assert(vmm_get_flags((vaddr_t)kernel_heap_start+0x1000) & VMM_PTE_FLAG_PRESENT);
    assert(!(vmm_get_flags((vaddr_t)kernel_heap_start+0x2000) & VMM_PTE_FLAG_PRESENT));

    assert(brk((void *)0x202000) == 0);
    assert(kernel_heap_end == (void *)0x202000);
    assert(vmm_get_flags((vaddr_t)kernel_heap_start) & VMM_PTE_FLAG_PRESENT);
    assert(vmm_get_flags((vaddr_t)kernel_heap_start+0x1000) & VMM_PTE_FLAG_PRESENT);
    assert(!(vmm_get_flags((vaddr_t)kernel_heap_start+0x2000) & VMM_PTE_FLAG_PRESENT));

    assert(brk((void *)0x203000) == 0);
    assert(kernel_heap_end == (void *)0x203000);
    assert(vmm_get_flags((vaddr_t)kernel_heap_start) & VMM_PTE_FLAG_PRESENT);
    assert(vmm_get_flags((vaddr_t)kernel_heap_start+0x1000) & VMM_PTE_FLAG_PRESENT);
    assert(vmm_get_flags((vaddr_t)kernel_heap_start+0x2000) & VMM_PTE_FLAG_PRESENT);
    assert(!(vmm_get_flags((vaddr_t)kernel_heap_start+0x3000) & VMM_PTE_FLAG_PRESENT));

    // test decrementing break
    assert(brk((void *)0x201fff) == 0);
    assert(kernel_heap_end == (void *)0x201fff);
    assert(vmm_get_flags((vaddr_t)kernel_heap_start) & VMM_PTE_FLAG_PRESENT);
    assert(vmm_get_flags((vaddr_t)kernel_heap_start+0x1000) & VMM_PTE_FLAG_PRESENT);
    assert(!(vmm_get_flags((vaddr_t)kernel_heap_start+0x2000) & VMM_PTE_FLAG_PRESENT));

    assert(brk((void *)0x201000) == 0);
    assert(kernel_heap_end == (void *)0x201000);
    assert(vmm_get_flags((vaddr_t)kernel_heap_start) & VMM_PTE_FLAG_PRESENT);
    assert(!(vmm_get_flags((vaddr_t)kernel_heap_start+0x1000) & VMM_PTE_FLAG_PRESENT));

    assert(brk((void *)0x200000) == 0);
    assert(kernel_heap_end == (void *)0x200000);
    assert(!(vmm_get_flags((vaddr_t)kernel_heap_start) & VMM_PTE_FLAG_PRESENT));

    // test sbrk
    assert(sbrk(-1) == (void *)-1);

    assert(sbrk(1) == (void *)0x200000);
    assert(kernel_heap_end == (void *)0x200001);
    assert(vmm_get_flags((vaddr_t)kernel_heap_start) & VMM_PTE_FLAG_PRESENT);
    assert(!(vmm_get_flags((vaddr_t)kernel_heap_start+0x1000) & VMM_PTE_FLAG_PRESENT));

    assert(sbrk(0xfff) == (void *)0x200001);
    assert(kernel_heap_end == (void *)0x201000);
    assert(vmm_get_flags((vaddr_t)kernel_heap_start) & VMM_PTE_FLAG_PRESENT);
    assert(!(vmm_get_flags((vaddr_t)kernel_heap_start+0x1000) & VMM_PTE_FLAG_PRESENT));

    assert(sbrk(-0x1000) == (void *)0x201000);
    assert(kernel_heap_end == (void *)0x200000);
    assert(!(vmm_get_flags((vaddr_t)kernel_heap_start) & VMM_PTE_FLAG_PRESENT));
}*/
