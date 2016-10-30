#ifndef TASK_H
#define TASK_H
#include <stdint.h>
#include "mem/vmm.h"
typedef enum task_state {
    TASK_STATE_READY,
    TASK_STATE_WAIT
} task_state;

typedef struct task_struct {
    uint32_t pid;
    task_state state;
    pde_t *pagedir;
    struct task_struct *parent;
    struct task_struct *children;
    struct task_struct *next_child;
    struct task_struct *next_queue;
    char *cmdline;
} task_struct;

extern task_struct *current;
extern task_struct *kernel_task;
#endif
