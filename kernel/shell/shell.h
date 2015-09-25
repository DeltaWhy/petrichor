#ifndef SHELL_H
#define SHELL_H

#define MAX_BUILTINS 16
typedef void(*builtin_ptr)(int, char **);
typedef struct builtin {
    builtin_ptr fn;
    const char *name;
    const char *description;
} builtin;

void shell();
void register_builtin(builtin b);
void init_shell_builtins();
#endif
