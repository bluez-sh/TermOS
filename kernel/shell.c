#include "shell.h"
#include "../libc/string.h"
#include "../libc/file.h"
#include "../libc/util.h"
#include "../cpu/ports.h"

void shell_exec(char *cmd)
{
    if (!str_cmp(cmd, "poweroff")) {
        kprint("[+] Shutting down...\n");
        delay(0.6);
        port_w16(0x604, 0x2000);    // qemu s/w shutdown
    } else if (!str_cmp(cmd, "clear")) {
        clear_screen();
        kprint("SwaOS v1.0\n");
    } else if (!str_cmp(cmd, "getpage")) {
        char page_addr[16] = "";
        uint32_t page = kmalloc(10);
        hex_to_ascii(page, page_addr);
        kprint("\nPage address: ");
        kprint(page_addr);
        kprint("\n");
    } else if (!str_cmp(cmd, "freepage")) {
        free(10);
    } else if (!str_cmp_n(cmd, "echo ", 5)) {
        kprint(cmd+5);
    } else if (!str_cmp_n(cmd, "fread ", 6)) {
        char *buf = (char*) kmalloc(1024);
        int fd = ascii_to_int(cmd+6);
        read_file(fd, buf);
        kprint(buf);
        free(1024);
    } else if (!str_cmp_n(cmd, "fwrite ", 7)) {
        char *buf = (char*) kmalloc(1024);
        int fd, n;
        cmd += 7;

        fd = ascii_to_int(cmd);
        while (*cmd != ' ') cmd++;
        cmd++;

        mem_set(buf, 0, 1024);
        n = str_len(cmd);
        mem_cpy(cmd, buf, n);
        write_file(fd, buf);
        free(1024);
    } else {
        kprint("[!] Command not found");
    }
    kprint("\n>");
}
