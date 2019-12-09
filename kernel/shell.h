#ifndef KERNEL_SHELL_H
#define KERNEL_SHELL_H

#include "../libc/mem.h"
#include "../libc/string.h"
#include "../drivers/screen.h"

void shell_exec(char* cmd);

#endif
