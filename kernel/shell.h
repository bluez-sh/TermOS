#ifndef KERNEL_SHELL_H
#define KERNEL_SHELL_H

#include "../libc/mem.h"
#include "../libc/string.h"
#include "../drivers/screen.h"

void shell_parse_commands(char *str, int *cnt, char **cmd_list);
void shell_free_commands(int cnt, char **cmd_list);
void shell_run();
void shell_exec(char* cmd);

#endif
