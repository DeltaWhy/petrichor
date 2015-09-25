#ifndef SBRK_H
#define SBRK_H
int brk(void *addr);
void *sbrk(intptr_t increment);
#endif
