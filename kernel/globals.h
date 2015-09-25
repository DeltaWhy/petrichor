#ifndef GLOBALS_H
#define GLOBALS_H

#define UNUSED(x) (void)x

#ifdef USE_HLT
#define wait() __asm__ ("hlt")
#else
#define wait()
#endif

#ifndef NDEBUG
#define bochsbrk() __asm__ __volatile__ ("xchg %bx, %bx");
#else
#define bochsbrk()
#endif

void panic(const char *err);

#endif
