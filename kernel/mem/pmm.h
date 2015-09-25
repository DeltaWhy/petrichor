#ifndef PMM_H
#define PMM_H
#include <stdbool.h>
#include <stdint.h>
typedef uintptr_t paddr_t;
void init_pmm();
paddr_t pmm_alloc_frames(int n);
void pmm_free_frames(paddr_t addr, int n);
void pmm_reserve_frames(paddr_t addr, int n);
void pmm_reserve_range(paddr_t start, paddr_t end);
bool pmm_is_free(paddr_t addr);
#endif
