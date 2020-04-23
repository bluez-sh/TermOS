#ifndef PROGRAMS_PROGRAM_H
#define PROGRAMS_PROGRAM_H

#include "../libc/string.h"

#define MAX_PROGS 20

typedef struct prog_struct {
    char name[16];
    void (*main)(void);
} prog_struct_t;

// register the program structs
extern prog_struct_t prog_hello;

static prog_struct_t *prog_list[MAX_PROGS] = {
    &prog_hello,
    0,
};

int exec_program(char *name);

#endif
