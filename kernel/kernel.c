#include "kernel.h"
#include "../libc/string.h"
#include "../drivers/screen.h"
#include "../cpu/isr.h"

void kmain(void)
{
    isr_install();
    irq_install();

    kprint("SwaOS is running. Type 'poweroff' to halt the CPU.\n");
    kprint("\n>");
}

void user_input(char *input)
{
    if (!str_cmp(input, "poweroff")) {
        kprint("Stopping the CPU... Bye Bye!\n");
        __asm__ __volatile__("hlt");
    } else if (!str_cmp(input, "clear")) {
        clear_screen();
        kprint("SwaOS is running. Type 'poweroff' to halt the CPU.\n");
    }
    kprint("\n>");
}
