#include "shell.h"
#include "../libc/string.h"
#include "../libc/file.h"
#include "../libc/util.h"
#include "../cpu/ports.h"
#include "../cpu/timer.h"
#include "../libc/stdio.h"
#include "../fs/sfs.h"
#include "../programs/program.h"

#define NB_COMMANDS 17
static char *cmd_all[] = {
    "help", "poweroff", "clear", "getpage", "freepage", "echo",
    "fread<>", "fwrite<>", "gettick", "calc<>", "test_stdin",
    "debugfs", "formatfs", "mountfs", "fcreate", "fremove<>",
    "fstat<>"
};

void shell_run()
{
    char cmd[512];
    for (;;) {
        gets(cmd);
        shell_exec(cmd);
    }
}

void shell_exec(char *cmd)
{
    if (!str_cmp(cmd, "poweroff")) {
        kprint("[+] Shutting down...\n");
        delay(600);
        port_w16(0x604, 0x2000);    // qemu s/w shutdown

    } else if (!str_cmp(cmd, "clear")) {
        clear_screen();
        kprint("TermOS v1.0\n");

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
        char *buf = (char*) kmallocz(512);
        int fd = ascii_to_int(cmd+6);
        /*read_file(fd, buf);*/
        if (sfs_read(fd, buf, 512, 0) < 0)
            kprint("ERR: Cannot read file");
        else
            kprint(buf);
        free(512);

    } else if (!str_cmp_n(cmd, "fwrite ", 7)) {
        char *buf = (char*) kmallocz(512);
        int fd, n;
        cmd += 7;

        fd = ascii_to_int(cmd);
        while (*cmd != ' ') cmd++;
        cmd++;

        n = str_len(cmd);
        mem_cpy(cmd, buf, n);
        /*write_file(fd, buf);*/
        if (sfs_write(fd, buf, n, 0) < 0)
            kprint("ERR: Cannot write to file");
        free(512);

    } else if (!str_cmp(cmd, "gettick")) {
        kprintd(timer_get_ticks());

    } else if (!str_cmp_n(cmd, "calc ", 5)) {
        int op1, op2, res = 0;
        char opr, err = 0;
        cmd += 5;
        op1 = ascii_to_int(cmd);
        while (*cmd >= '0' && *cmd <= '9') cmd++;
        opr = *cmd++;
        op2 = ascii_to_int(cmd);

        switch (opr) {
            case '+': res = op1 + op2;  
                      break;
            case '-': res = op1 - op2;
                      break;
            case '*': res = op1 * op2;
                      break;
            case '/':
                      if (op2 == 0) {
                          kprint("ERR: Division by 0");
                          err = 1;
                      } else {
                          res = op1 / op2;
                      }
                      break;
            case '%': res = op1 % op2;
                      break;
            default:
                      kprint("ERR: Invalid Operator");
                      err = 1;
        }
        if (!err)
            kprintd(res);

    } else if (!str_cmp(cmd, "test_stdin")) {
        char str[64];
        do {
            gets(str);
            kprint(str);
            kprint("\n");
        } while (str[0] != '0');

    } else if (!str_cmp(cmd, "debugfs")) {
        sfs_debug();

    } else if (!str_cmp(cmd, "formatfs")) {
        sfs_format();

    } else if (!str_cmp(cmd, "mountfs")) {
        if (sfs_mount())
            kprint("\nERR: Cannot mount filesystem");

    } else if (!str_cmp(cmd, "fcreate")) {
        int ret = sfs_create();
        if(ret < 0) {
            kprint("\nERR: Cannot create file");
        } else {
            kprint("File created with fd: ");
            kprintd(ret);
        }

    } else if (!str_cmp_n(cmd, "fremove ", 8)) {
        cmd += 8; 
        int fd = ascii_to_int(cmd);
        if (sfs_remove(fd) < 0)
            kprint("\nERR: Cannot remove file");

    } else if (!str_cmp_n(cmd, "fstat ", 6)) {
        cmd += 6;
        int fd = ascii_to_int(cmd);
        int ret = sfs_stat(fd);
        if (ret < 0) {
            kprint("\nERR: File does not exist");
        } else {
            kprint("\nFile size: ");
            kprintd(ret);
        }

    } else if (!str_cmp(cmd, "help")) {
        kprint("Commands: ");
        int i;
        for (i = 0; i < NB_COMMANDS-1; i++) {
            kprint(cmd_all[i]);
            kprint(", ");
            if (i && i % 7 == 0)
                kprint("\n");
        }
        kprint(cmd_all[i]);

    } else {
        if (!exec_program(cmd))
            kprint("[!] Command not found");
    }
    kprint("\n>");
}
