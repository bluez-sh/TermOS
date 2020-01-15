#include "kernel.h"
#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "shell.h"
#include "../drivers/ata.h"
#include "../libc/util.h"

void kmain(void)
{
    isr_install();
    irq_install();

    ata_init();
    ata_scan_drives();

    delay(1.5);
    clear_screen();
    kprint("\nWelcome to SwaOS!\n");
    kprint("\n>");
}

void user_input(char *input)
{
    shell_exec(input);
}
