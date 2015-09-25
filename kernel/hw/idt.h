#ifndef IDT_H
#define IDT_H
#include <stdint.h>

#define IDT_TASK_32 0x5
#define IDT_INT_16 0x6
#define IDT_TRAP_16 0x7
#define IDT_INT_32 0xE
#define IDT_TRAP_32 0xF
#define IDT_SEG_STORAGE 0x10
#define IDT_DPL0 0x0
#define IDT_DPL1 0x20
#define IDT_DPL2 0x40
#define IDT_DPL3 0x60
#define IDT_PRESENT 0x80

struct IDTDescr {
    uint16_t offset_1; // offset bits 0..15
    uint16_t selector; // a code segment selector in GDT or LDT
    uint8_t zero;      // unused, set to 0
    uint8_t type_attr; // type and attributes, see below
    uint16_t offset_2; // offset bits 16..31
};

struct IDTDescr IDT[48];
void setIdt(void *ptr, uint16_t size);
void init_idt();
#endif
