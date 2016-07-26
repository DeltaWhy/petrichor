#include <stdint.h>
#include <string.h>
#include "globals.h"
#include "mboot.h"
#include "sprintf.h"
#include "hw/acpi.h"
#include "hw/idt.h"
#include "hw/keyboard.h"
#include "hw/pic.h"
#include "hw/pit.h"
#include "hw/port.h"
#include "hw/screen.h"
#include "mem/gdt.h"
#include "mem/pmm.h"
#include "mem/stacktrace.h"
#include "mem/vmm.h"
#include "shell/shell.h"
void exit(void);

extern const char kernel_start[];
extern const char kernel_end[];

void kmain(uint32_t magic)
{
    if ( magic != 0x2BADB002 ) {
        print("Something went not according to specs.");
        exit();
    }

    init_screen();
    char printbuf[256];
    snprintf(printbuf, 256, "kernel loaded at %p, ends at %p\n", kernel_start, kernel_end);
    print(printbuf);
    print("initializing GDT...\n");
    init_gdt();
    print("initializing IDT...\n");
    init_idt();
    print("initializing physical memory manager...\n");
    init_pmm();
    if (pmm_is_free((paddr_t)kernel_start) || pmm_is_free((paddr_t)kernel_end)) panic("kernel memory is not reserved");
    if (pmm_is_free((paddr_t)0xb8000)) panic("video ram is not reserved");
    print("initializing virtual memory manager...\n");
    init_vmm();
    print("initializing PICs...\n");
    init_pics(0x20, 0x28);
    print("initializing keyboard...\n");
    init_keyboard();
    print("enabling keyboard interrupts...\n");
    enable_irq(1);
    send_eoi(0);
    __asm__ __volatile__ ("sti");
    print("initializing symbol table...\n");
    init_stacktrace();
    print("initializing timer...\n");
    init_timer(10);
    enable_irq(0);
    print("initializing speaker...\n");
    print("initializing ACPI...\n");
    init_acpi();
    print("reclaiming ACPI memory...\n");
    acpi_reclaim_memory();
    print("initializing shell...\n");
    init_shell_builtins();
    beep(100, 100);
    cprint("Petrichor OS\n", 2);
    update_cursor();
    shell();
}
void exit(void)
{
    while (inportb(0x64) & 0x02);
    outportb(0x64, 0xFE);
}

void panic(const char *err)
{
    cprint("KERNEL PANIC: ", 0x04);
    printf("%s\n", err);
    stacktrace();
    // magic breakpoint for bochs debugger
    bochsbrk();
    __asm__ __volatile__ ("cli");
    while (1) __asm__ ("hlt");
}
