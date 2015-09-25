#include "globals.h"
#include "isr.h"
#include "screen.h"
#include "pic.h"
#include "keyboard.h"
#include "pit.h"
#include "mem/stacktrace.h"
void isr_handler(registers_t regs)
{
    void *cr2;
    switch (regs.int_no) {
    case 0:
        // DIV0
        isr_panic("Divide by zero", regs);
        break;
    case 13:
        // GPF
        isr_panic("General protection fault", regs);
        break;
    case 14:
        __asm__ ("movl %%cr2, %0"
                 : "=a"(cr2));
        printf("cr2=%p\n", cr2);
        isr_panic("Page fault", regs);
        break;
    case 32:
        // timer
        // are we really sure this should be 32?
        timer_handler(regs);
        send_eoi(0);
        break;
    case 33:
        // keyboard
        keyboard_irq_handler();
        send_eoi(1);
        break;


    default:
        printf("interrupt %d\n", regs.int_no);
        break;
    }
}

void isr_panic(const char *err, registers_t regs)
{
    cprint("KERNEL PANIC: ", 0x04);
    printf("%s (error code %d)\n", err, regs.err_code);
    printf("register dump:\n"
           "    eax=%#.8x\n"
           "    ebx=%#.8x\n"
           "    ecx=%#.8x\n"
           "    edx=%#.8x\n"
           "    esp=%#.8x\n"
           "    ebp=%#.8x\n"
           "    esi=%#.8x\n"
           "    edi=%#.8x\n"
           "    cs=%#x ds=%#x\n"
           "    eflags=%#.8x\n"
           "    eip=%#.8x\n",
           regs.eax, regs.ebx, regs.ecx, regs.edx, regs.esp, regs.ebp,
           regs.esi, regs.edi, regs.cs, regs.ds, regs.eflags, regs.eip);
    stacktrace_from((void *)regs.eip, (void **)regs.ebp);
    bochsbrk();
    __asm__ __volatile__ ("cli");
    while (1) __asm__ ("hlt");
}
