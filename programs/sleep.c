#include "program.h"
#include "../libc/stdio.h"
#include "../libc/util.h"
#include "../libc/string.h"

int sleep_main(int argc, char **argv)
{
    if (argc < 2) {
        kprints("sleep: missing operand\n");
        return -1;
    }

    unsigned ms = 0;
    for (int i = 1; i < argc; i++)
        ms += ascii_to_int(argv[i]);

    delay(ms);
    return 0;
}

prog_struct_t prog_sleep = {
    .name = "sleep",
    .main = sleep_main,
};
