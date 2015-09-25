#ifndef PIT_H
#define PIT_H
#include "isr.h"
#define PIT_CHANNEL0      0x40  //PIT Channel 0's Data Register Port
#define PIT_CHANNEL1      0x41  //PIT Channels 1's Data Register Port, we wont be using this here
#define PIT_CHANNEL2     0x42  //PIT Channels 2's Data Register Port
#define PIT_CMDREG        0x43  //PIT Chip's Command Register Port
void init_timer(int hz);
void init_speaker(int hz);
void timer_handler(registers_t regs);
void delay(int time);
void play_sound(unsigned long hz);
void no_sound();
void beep(unsigned long length,unsigned long hz);
#endif
