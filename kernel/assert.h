#ifndef ASSERT_H
#define ASSERT_H
#include "sprintf.h"
#include "hw/screen.h"
#include "globals.h"

#ifdef NDEBUG
#define assert(cond) ((void)0)
#else
#define assert(cond) do { \
    if (!(cond)) { \
        printf("%s:%d: %s: Assertion `"#cond"' failed.\n", __FILE__, __LINE__, __func__); \
        panic("Assertion failed"); \
    } } while (0)
#endif
#endif
