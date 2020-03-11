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

    for (int i = 3; i >= 0; i--)
        if (ata_drive_select(i%2, i/2) >= 0)
            break;

    delay(1.5);
    clear_screen();
    kprint("\nWelcome to SwaOS!\n");
    kprint("\n>");

    shell_run();
}
