#include <stddef.h>
#include "task/task.h"

static task_struct ktask = (task_struct) {
    .pid = 0,
    .state = TASK_STATE_READY,
    .pagedir = NULL,
    .parent = NULL,
    .children = NULL,
    .next_child = NULL,
    .next_queue = NULL,
    .cmdline = "[kernel]"
};
task_struct *current = &ktask;
task_struct *kernel_task = &ktask;
