#include "sched.h"
#include "../drivers/screen.h"
#include "../libc/stdio.h"
#include "../libc/string.h"
#include "../programs/program.h"

static int task_view = 1;

static volatile struct task task_queue[MAX_TASKS];
static volatile int front = -1;
static volatile int rear  = -1;

static int task_queue_add(char *cmd)
{
    if (front == rear + 1 || (front == 0 && rear == MAX_TASKS - 1))
        return -1;

    if (front == -1) front = 0;
    rear = (rear + 1) % MAX_TASKS;

    task_queue[rear].id = alloc_task_id++;
    str_cpy((char*) task_queue[rear].cmd, cmd);
    return 0;
}

static int task_queue_remove(volatile struct task **task)
{
    if (front == -1)
        return -1;
    *task = &task_queue[front];

    if (front == rear)
        front = rear = -1;
    else
        front = (front + 1) % MAX_TASKS;
    return 0;
}

void toggle_task_view()
{
    task_view = !task_view;
}

// To be called on interrupt only
void prompt_add_program()
{
    char cmd[512];

    /*kprint("\n[!] SCHED: Task Interrupted\n");*/
    /*kprint("[+] SCHED: Add new command\n");*/
    kprint("\nEnqueue> ");

    // We are in interrupt context
    __asm__ __volatile__("sti");
    gets(cmd);
    __asm__ __volatile__("cli");

    if (task_queue_add(cmd) < 0)
        kprint("\n[!] SCHED: Maximum tasks exceeded");

    kprint("\n>");
    /*kprint("\n[+] SCHED: Command added to task queue\n");*/
}

void exec_task_queue()
{
    volatile struct task *next_task = 0;

    while (task_queue_remove(&next_task) >= 0) {
        if (task_view) {
            kprint("\n\n[+] Task #");
            kprintd(next_task->id);
            kprint("\n======================================\n");
        }

        if (!exec_program((char*) next_task->cmd)) {
            kprint("[!] Command \'");
            kprint((char*) next_task->cmd);
            kprint("\' not found\n");
        }
    }
    if (task_view)
        kprint("\n");
}

void sched_new_task(char *cmd, int exec_now)
{
    if (task_queue_add(cmd) < 0) {
        kprint("\n[!] SCHED: Maximum tasks exceeded");
        return;
    }
    if (exec_now)
        exec_task_queue();
}
