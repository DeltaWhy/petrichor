#include "gdt.h"
static gdt_descriptor create_descriptor(uint32_t base, uint32_t limit, uint16_t flag);

void init_gdt(void)
{
    /*GDT[0] = ((sizeof(gdt_descriptor) * GDT_ENTRIES - 1) & 0xFFFF)
            | ((uint32_t)GDT) << 16;*/
    GDT[1] = create_descriptor(0, 0xFFFFF, GDT_CODE_PL0);
    GDT[2] = create_descriptor(0, 0xFFFFF, GDT_DATA_PL0);
    setGdt(GDT, sizeof(GDT));
    reloadSegments();
}
static gdt_descriptor create_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    gdt_descriptor descriptor;

    // Create the high 32 bit segment
    descriptor  =  limit       & 0x000F0000;         // set limit bits 19:16
    descriptor |= (flag <<  8) & 0x00F0FF00;         // set type, p, dpl, s, g, d/b, l and avl fields
    descriptor |= (base >> 16) & 0x000000FF;         // set base bits 23:16
    descriptor |=  base        & 0xFF000000;         // set base bits 31:24

    // Shift by 32 to allow for low part of segment
    descriptor <<= 32;

    // Create the low 32 bit segment
    descriptor |= base  << 16;                       // set base bits 15:0
    descriptor |= limit  & 0x0000FFFF;               // set limit bits 15:0

    return descriptor;
}
