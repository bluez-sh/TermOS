#include "kernel.h"
#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "shell.h"

void kmain(void)
{
    isr_install();
    irq_install();

    kprint("SwaOS is running. Type 'poweroff' to halt the CPU.\n");
    kprint("\n>");
}

void user_input(char *input)
{
    shell_exec(input);
}
