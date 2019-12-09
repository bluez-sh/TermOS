#include "shell.h"

void shell_exec(char *cmd)
{
    if (!str_cmp(cmd, "poweroff")) {
        kprint("Stopping the CPU... Bye Bye!\n");
        __asm__ __volatile__("hlt");
    } else if (!str_cmp(cmd, "clear")) {
        clear_screen();
        kprint("SwaOS is running. Type 'poweroff' to halt the CPU.\n");
    } else if (!str_cmp(cmd, "getpage")) {
        char page_addr[16] = "";
        uint32_t page = kmalloc(4000);
        hex_to_ascii(page, page_addr);
        kprint("\nPage address: ");
        kprint(page_addr);
        kprint("\n");
    }
    kprint("\n>");
}
