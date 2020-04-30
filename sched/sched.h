#ifndef SCHED_SCHED_H
#define SCHED_SCHED_H

#include "../cpu/types.h"

#define CMD_SIZE 512

static uint8_t alloc_task_id = 0;

struct task {
    uint8_t id;
    char cmd[CMD_SIZE];
};

void toggle_task_view();
void prompt_add_program();
void exec_task_queue();
void sched_new_task();

#endif
