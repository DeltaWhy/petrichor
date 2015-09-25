#include "port.h"
#include "screen.h"
#include <string.h>
#include "isr.h"
#include "../globals.h"
#include "pit.h"
#include <stdint.h>
unsigned long counter = 0;

void init_speaker(int hz)
{
    int divisor = 1193180 / hz;
    outportb(PIT_CMDREG, 0xb6);
    outportb(PIT_CHANNEL2, (unsigned char) (divisor) );
    outportb(PIT_CHANNEL2, (unsigned char) (divisor >> 8));
}

void init_timer(int hz)
{
    int divisor = 1193180 / hz;
    outportb(PIT_CMDREG , 0x36);
    outportb(PIT_CHANNEL0, divisor & 0xFF);
    outportb(PIT_CHANNEL0, divisor >> 8);
}


void timer_handler(registers_t regs)
{
    UNUSED(regs);
    //printf("%x\n",counter);


    counter++;
}

void delay(int count)
{
    unsigned long wait_till;//This is addition of count and the current no of ticks

    wait_till = counter + count;
    //printf("%d\n",counter);
    while (counter < wait_till); //loops until the given period has been reached
}


void play_sound(unsigned long hz)
{
    int divisor = 1193180 / hz;
    outportb(PIT_CMDREG , 0x36);
    outportb(PIT_CHANNEL0, divisor & 0xFF);
    outportb(PIT_CHANNEL0, divisor >> 8);

    unsigned short tmp;
    UNUSED(tmp);
    tmp = inportb(0x61);
    if (tmp != (tmp | 3)) {
        outportb(0x61, tmp | 3);

    }
}

void nosound()
{
    unsigned short tmp=(inportb(0x61) & (0xFC));
    UNUSED(tmp);
    outportb(0x61, tmp);



}

void beep(unsigned long length,unsigned long hz)
{
    play_sound(hz);
    delay(length);
    nosound();
}
