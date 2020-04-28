#include "program.h"
#include "../drivers/screen.h"
#include "../libc/mem.h"
#include "../libc/string.h"

#define MAX_ARGS 20

static char* next_token(char **cmd_ptr)
{
    char *ptr = *cmd_ptr;
    int len = 0;

    if (*ptr == '\0')
        return 0;
    while (*ptr == ' ') ptr++;

    char *start = ptr;

    if (*ptr == '\"')
        for (ptr++, start++; *ptr != '\"' && *ptr != '\0'; ptr++, len++);
    else
        for (; *ptr != ' ' && *ptr != '\0'; ptr++, len++);

    char *str = (char*) kmalloc(len + 1);
    mem_cpy(start, str, len);
    str[len] = '\0';

    if (*ptr == '\"') ptr++;
    while (*ptr == ' ') ptr++;
    *cmd_ptr = ptr;
    return str;
}

static void parse_args(char *cmd, int *argc, char **argv)
{
    int i = 0;
    char *cmd_ptr = cmd;
    while ((argv[i] = next_token(&cmd_ptr)) != 0) i++;
    *argc = i;
}

static void free_args(int argc, char **argv)
{
    for (int i = 0; i < argc; i++)
        free(str_len(argv[i]) + 1);
}

int exec_program(char *cmd)
{
    int argc;
    char *argv[MAX_ARGS];

    parse_args(cmd, &argc, argv);

    for (int i = 0; i < MAX_PROGS && prog_list[i]; i++) {
        if (!str_cmp(argv[0], prog_list[i]->name)) {
            prog_list[i]->main(argc, argv);
            return 1;
        }
    }

    free_args(argc, argv);
    return 0;
}

void print_program_names()
{
    for (int i = 0; i < MAX_PROGS && prog_list[i]; i++) {
        kprint(prog_list[i]->name);
        kprint(", ");
    }
    kprint_backspace();
    kprint_backspace();
}
