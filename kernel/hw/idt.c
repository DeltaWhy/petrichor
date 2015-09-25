#include "idt.h"
#include "isr.h"
static void set_idt_descriptor(int irq, void *handler, uint16_t selector, uint8_t type_attr);

void init_idt()
{
    for (unsigned int i = 0; i < sizeof(IDT); i++) {
        *(((char *)IDT)+i) = 0;
    }
    set_idt_descriptor(0, _isr_0, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(1, _isr_1, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(2, _isr_2, 0x08, IDT_DPL0 | IDT_INT_32); //Intel reserved
    set_idt_descriptor(3, _isr_3, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(4, _isr_4, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(5, _isr_5, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(6, _isr_6, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(7, _isr_7, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(8, _isr_8, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(9, _isr_9, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(10, _isr_10, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(11, _isr_11, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(12, _isr_12, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(13, _isr_13, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(14, _isr_14, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(15, _isr_15, 0x08, IDT_DPL0 | IDT_INT_32); //Intel reserved
    set_idt_descriptor(16, _isr_16, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(17, _isr_17, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(18, _isr_18, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(19, _isr_19, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(20, _isr_20, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(21, _isr_21, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(22, _isr_22, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(23, _isr_23, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(24, _isr_24, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(25, _isr_25, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(26, _isr_26, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(27, _isr_27, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(28, _isr_28, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(29, _isr_29, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(30, _isr_30, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(31, _isr_31, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(32, _isr_32, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(33, _isr_33, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(34, _isr_34, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(35, _isr_35, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(36, _isr_36, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(37, _isr_37, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(38, _isr_38, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(39, _isr_39, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(40, _isr_40, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(41, _isr_41, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(42, _isr_42, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(43, _isr_43, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(44, _isr_44, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(45, _isr_45, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(46, _isr_46, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    set_idt_descriptor(47, _isr_47, 0x08, IDT_DPL0 | IDT_INT_32 | IDT_PRESENT);
    setIdt(IDT, sizeof(IDT));
}
static void set_idt_descriptor(int irq, void *handler, uint16_t selector, uint8_t type_attr)
{
    IDT[irq].offset_1 = ((uint32_t)handler) & 0xFFFF;
    IDT[irq].offset_2 = ((uint32_t)handler) >> 16;
    IDT[irq].selector = selector;
    IDT[irq].type_attr = type_attr;
}
