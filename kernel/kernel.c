#include "kernel.h"
#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "shell.h"
#include "../drivers/ata.h"
#include "../libc/util.h"
#include "../fs/sfs.h"

void kmain(void)
{
    isr_install();
    irq_install();

    ata_init();
    ata_scan_drives();

    for (int i = 3; i >= 0; i--)
        if (ata_drive_select(i%2, i/2) >= 0)
            break;

    if (sfs_mount() < 0)
        kprint("\n[!] SFS: Error mounting drive");
    else
        kprint("\n[+] SFS: Drive mount success");

    delay(2000);

    clear_screen();
    kprint("\nWelcome to TermOS!\n");
    kprint("\n>");

    shell_run();
}
