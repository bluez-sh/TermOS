#include "program.h"
#include "../libc/stdio.h"

void main()
{
    kprints("Hello, World!\n");
}

prog_struct_t prog_hello = {
    .name = "hello",
    .main = main,
};
