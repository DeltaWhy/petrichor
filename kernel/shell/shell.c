#include <stdbool.h>
#include <string.h>
#include "globals.h"
#include "shell.h"
#include "hw/keyboard.h"
#include "hw/screen.h"
#include "hw/pit.h"
#include "mem/stacktrace.h"
#include "hw/acpi.h"
static builtin builtins[MAX_BUILTINS];
static int num_builtins = 0;

void shell()
{
    kbd_set_mode(KBD_INFO_MODE_ECHO | KBD_INFO_MODE_LINE);
    char input[1024];
    while (1) {
        cprint("> ", 0x0A);
        kgets(input);

        // tokenize input
        int argc=0;
        char *argv[100];
        argv[0] = strtok(input, " ");
        for (int i=0; argv[i] != NULL; i++) {
            argv[i+1] = strtok(NULL, " ");
            argc++;
        }

        bool found = false;
        for (int i=0; i < num_builtins; i++) {
            if (strcmp(argv[0], builtins[i].name) == 0) {
                found = true;
                (*(builtins[i].fn))(argc, argv);
                break;
            }
        }
        if (!found) {
            if (*input==0);

            else
                printf("%s: command not found\n", input);
        }
        continue;
    }
}

void register_builtin(builtin b)
{
    builtins[num_builtins] = b;
    num_builtins++;
}

static void help(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);
    print("supported commands:\n");
    for (int i=0; i < num_builtins; i++) {
        printf("    %s - %s\n", builtins[i].name, builtins[i].description);
    }
}
builtin help_builtin = {&help, "help", "Prints this message."};

static void echo(int argc, char **argv)
{
    for (int i=1; i < argc; i++) {
        print(argv[i]);
        if (i < argc-1) print(" ");
    }
    print("\n");
}
builtin echo_builtin = {&echo, "echo", "Prints the arguments given."};

static void shell_beep(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    beep(1000, 1000);
}


builtin shell_beep_builtin = {&shell_beep, "beep", "try it and find out."};



static void clear(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);
    kclear();
}
builtin clear_builtin = {&clear, "clear", "Clears the screen."};

static void interrupt(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);
    print("interrupting...\n");
    __asm__ __volatile__ ("int $3");
}
builtin interrupt_builtin = {&interrupt, "interrupt", "Tests software interrupts."};

static void crash(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);
    print("dividing by 0...\n");
    int x, y, z;
    x = 10;
    y = 0;
    z = x / y;
    x = z;
}
builtin crash_builtin = {&crash, "crash", "Tests CPU exception handling by dividing by 0."};

static void test_panic(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);
    print("panicking...\n");
    panic("panic test");
    print("this should never print");
}
builtin panic_builtin = {&test_panic, "panic", "Tests kernel panic function."};

static void timer(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);
    // __asm__ __volatile__ ("int $32");
    delay(1000);
}
builtin timer_builtin = {&timer, "timer", "should run 1000 ticks on the timer. (1 sec at 10hz)"};



extern char *kernel_heap_start;
extern char *kernel_heap_end;
static void heap(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);
    printf("kernel heap start: %p\n"
           "              end: %p\n", kernel_heap_start, kernel_heap_end);
}
builtin heap_builtin = {&heap, "heap", "Show size of the kernel heap."};

void exit(void);
static void shell_exit(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);
    exit();
}
builtin exit_builtin = {&shell_exit, "exit", "Shutdown the system."};

static void test_stacktrace(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);
    stacktrace();
}
builtin stacktrace_builtin = {&test_stacktrace, "stacktrace", "Test the stacktrace function."};

static void shutdown(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);
    acpi_shutdown();
}
builtin shutdown_builtin = {&shutdown, "shutdown", "Do an ACPI shutdown."};

extern builtin bootinfo_builtin;
extern builtin memmap_builtin;
extern builtin loadkeys_builtin;

void init_shell_builtins()
{
    register_builtin(help_builtin);
    register_builtin(echo_builtin);
    register_builtin(clear_builtin);
    register_builtin(interrupt_builtin);
    register_builtin(crash_builtin);
    register_builtin(panic_builtin);
    register_builtin(bootinfo_builtin);
    register_builtin(memmap_builtin);
    register_builtin(loadkeys_builtin);
    register_builtin(heap_builtin);
    register_builtin(timer_builtin);
    register_builtin(shell_beep_builtin);
    register_builtin(exit_builtin);
    register_builtin(stacktrace_builtin);
    register_builtin(shutdown_builtin);
}


