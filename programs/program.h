#ifndef PROGRAMS_PROGRAM_H
#define PROGRAMS_PROGRAM_H

#include "../libc/string.h"

#define MAX_PROGS 20

typedef struct prog_struct {
    char name[16];
    int (*main)(int, char**);
} prog_struct_t;

// register the program structs
extern prog_struct_t prog_echo;
extern prog_struct_t prog_calculator;
extern prog_struct_t prog_sleep;

static prog_struct_t *prog_list[MAX_PROGS] = {
    &prog_echo,
    &prog_calculator,
    &prog_sleep,
    0,
};

int exec_program(char *cmd);
void print_program_names();

#endif
