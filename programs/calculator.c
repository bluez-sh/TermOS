#include "program.h"
#include "../libc/stdio.h"
#include "../libc/string.h"

int calc_main(int argc, char **argv)
{
    if (argc < 4) {
        kprints("Too few arguments\n");
        return -1;
    } else if (argc > 4) {
        kprints("Too many arguments\n");
        return -1;
    }

    int op1 = ascii_to_int(argv[1]);
    char *opr = argv[2];
    int op2 = ascii_to_int(argv[3]);
    int res = 0;

    switch (*opr) {
    case '+':   res = op1 + op2;  
                break;
    case '-':   res = op1 - op2;
                break;
    case '*':   res = op1 * op2;
                break;
    case '/':   if (op2 == 0) {
                   kprints("Error: Division by 0\n");
                   return -1;
                }
                res = op1 / op2;
                break;
    case '%':   res = op1 % op2;
                break;
    case '^':   res = 1;
                while (op2--)
                    res *= op1;
                break;
    default:    kprints("Error: Invalid Operator\n");
                return -1;
    }

    kprintd(res);
    kprints("\n");
    return 0;
}

prog_struct_t prog_calculator = {
    .name = "calc",
    .main = calc_main,
};
