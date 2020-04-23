#include "program.h"

int exec_program(char *name)
{
    for (int i = 0; i < MAX_PROGS && prog_list[i]; i++) {
        if (!str_cmp(name, prog_list[i]->name)) {
            prog_list[i]->main();
            return 1;
        }
    }
    return 0;
}
