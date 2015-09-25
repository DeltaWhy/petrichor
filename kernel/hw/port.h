#ifndef PORT_H
#define PORT_H

#ifndef NDEBUG
#define bochslog(x) do { char *p = (char*)(x); while(*p) outportb(0xe9,*p++); } while(0);
#else
#define bochslog(x)
#endif

void outportb(unsigned port, unsigned val);
unsigned inportb(unsigned short port);
void outportw(unsigned port, unsigned val);
unsigned inportw(unsigned short port);

#endif
