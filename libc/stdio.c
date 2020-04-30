#include "stdio.h"
#include "string.h"
#include "util.h"
#include "mem.h"
#include "../drivers/screen.h"

#define STDIN_SIZE 512

static char stdin_buf[STDIN_SIZE];
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
    mem_set(stdin_buf, 0, STDIN_SIZE);
    stdin_ready = 0;
    __asm__ __volatile__("sti");
}

void kprintd(int num)
{
    char str[16];
    int_to_ascii(num, str);
    kprint(str);
}

void kprints(char *str)
{
    kprint(str);
}
