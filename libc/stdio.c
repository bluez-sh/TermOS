#include "stdio.h"
#include "string.h"
#include "util.h"

static char stdin_buf[512];
static volatile int stdin_ready;

void copy_to_stdin(char *input)
{
    str_cpy(stdin_buf, input);
    stdin_ready = 1;
}

void gets(char *str)
{
    while (!stdin_ready)
        ;
    __asm__ __volatile__("cli");
    str_cpy(str, stdin_buf);
    stdin_ready = 0;
    __asm__ __volatile__("sti");
}
