#include "program.h"
#include "../libc/stdio.h"
#include "../libc/string.h"

int echo_main(int argc, char **argv)
{
    char str[512] = {0};
    char *ptr = str;

    for (int i = 1; i < argc; i++) {
        str_cpy(ptr, argv[i]);
        append(ptr, ' ');
        ptr += str_len(argv[i]) + 1;
    }

    kprints(str);
    kprints("\n");
    return 0;
}

prog_struct_t prog_echo = {
    .name = "echo",
    .main = echo_main,
};
