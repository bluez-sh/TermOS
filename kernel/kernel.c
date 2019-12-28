#include "kernel.h"
#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "shell.h"
#include "../drivers/ata.h"

void kmain(void)
{
    isr_install();
    irq_install();

    ata_init();
    ata_scan_drives();

    ata_drive_select(1, 1);

    char buf[512] = "\nHurray!! It worked!!!";
    ata_write_sector(0, (uint8_t*)buf);
    mem_set(buf, 0, 512);

    ata_read_sector(0, (uint8_t*)buf);
    kprint(buf);

    kprint("\nSwaOS is running. Type 'poweroff' to halt the CPU.\n");
    kprint("\n>");
}

void user_input(char *input)
{
    shell_exec(input);
}
