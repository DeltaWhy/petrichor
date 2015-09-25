#include "pic.h"
#include "port.h"
#include <stdint.h>

void init_pics(int pic1, int pic2)
{
    /* send ICW1 */
    outportb(PIC1, ICW1);
    outportb(PIC2, ICW1);

    /* send ICW2 */
    outportb(PIC1 + 1, pic1);	/* remap */
    outportb(PIC2 + 1, pic2);	/*  pics */

    /* send ICW3 */
    outportb(PIC1 + 1, 4);	/* IRQ2 -> connection to slave */
    outportb(PIC2 + 1, 2);

    /* send ICW4 */
    outportb(PIC1 + 1, ICW4);
    outportb(PIC2 + 1, ICW4);

    /* disable all IRQs */
    outportb(PIC1 + 1, 0xFF);
    outportb(PIC2 + 1, 0xFF);
}

uint16_t ocw1 = 0xFFFF;

void enable_irq(int irq)
{
    ocw1 &= ~(1 << irq);
    if (irq < 8) outportb(PIC1+1, ocw1&0xFF);
    else outportb(PIC2+1, ocw1>>8);
}

void disable_irq(int irq)
{
    ocw1 |= (1 << irq);
    if (irq < 8) outportb(PIC1+1, ocw1&0xFF);
    else outportb(PIC2+1, ocw1>>8);
}

void send_eoi(int irq)
{
    if (irq >= 8) outportb(PIC2, 0x20);
    outportb(PIC1, 0x20);
}
