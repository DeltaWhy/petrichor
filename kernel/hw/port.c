#include "port.h"

void outportb(unsigned port, unsigned val)
{

    __asm__ __volatile__("outb %b0,%w1"


                         :

                         : "a"(val), "d"(port));

}

unsigned inportb(unsigned short port)
{

    unsigned char ret_val;

    __asm__ __volatile__("inb %1,%0"

                         : "=a"(ret_val)

                         : "d"(port));

    return ret_val;

}

void outportw(unsigned port, unsigned val)
{

    __asm__ __volatile__("outw %w0,%w1"


                         :

                         : "a"(val), "d"(port));

}

unsigned inportw(unsigned short port)
{

    unsigned short ret_val;

    __asm__ __volatile__("inb %1,%0"

                         : "=a"(ret_val)

                         : "d"(port));

    return ret_val;

}
