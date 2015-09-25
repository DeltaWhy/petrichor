#include "globals.h"
#include "shell/shell.h"
#include "hw/screen.h"
#include "hw/keyboard.h"

static void loadkeys(int argc, char **argv)
{
    if (argc == 2 && load_keys(argv[1])) return;
    printf("Usage: %s (standard | dvorak)\n", argv[0]);
}
builtin loadkeys_builtin = {&loadkeys, "loadkeys", "Sets keyboard layout. [layout]"};
