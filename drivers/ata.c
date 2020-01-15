#include "ata.h"
#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "../libc/function.h"
#include "../libc/string.h"
#include "../libc/mem.h"

static int is_primary;
static int is_master;
static int base;
static int to_select = 1;

static uint16_t ata_buffer[256];

/*
 * 11 => primary master
 * 10 => primary slave
 * 01 => secondary master
 * 00 => secondary slave
 */
static int drive_present[2][2];


static void ata_primary_handler(registers_t *r)
{
    UNUSED(r);
}

static void ata_secondary_handler(registers_t *r)
{
    UNUSED(r);
}

void ata_init()
{
    register_int_hdlr(IRQ14, ata_primary_handler);
    register_int_hdlr(IRQ15, ata_secondary_handler);
}

static int identify(int primary, int master)
{
    uint8_t status;

    base = primary ? ATA_PRIMARY_BASE : ATA_SECONDARY_BASE;

    status = port_r8(base + COMMAND_PORT);
    if (status == 0xFF)
        return 0;

    port_w8(base + DEVICE_PORT, master ? 0xA0 : 0xB0);
    port_w8(base + SECTOR_COUNT_PORT, 0);
    port_w8(base + LBA_LOW_PORT, 0);
    port_w8(base + LBA_MID_PORT, 0);
    port_w8(base + LBA_HI_PORT,  0);

    port_w8(base + COMMAND_PORT, 0xEC);
    status = port_r8(base + COMMAND_PORT);
    if (!status)
        return 0;

    kprint("\n[+] ATA: Identifying drive...");

    // wait for BSY clear
    while ((status & 0x80) == 0x80)
        status = port_r8(base + COMMAND_PORT);

    // wait for DRQ set or ERR set
    while ((status & 0x08) != 0x08 &&
           (status & 0x01) != 0x01)
        status = port_r8(base + COMMAND_PORT);

    if (status & 0x01) {
        kprint("\n[!] ATA: DRIVE ERROR");
        return 0;
    }

    for (int i = 0; i < 256; i++)
        ata_buffer[i] = port_r16(base + DATA_PORT);

    return 1;
}

void ata_scan_drives()
{
    if((drive_present[1][1] = identify(1, 1)) == 1)
        kprint("\n[+] ATA: Primary Master Drive detected");
    if((drive_present[1][0] = identify(1, 0)) == 1)
        kprint("\n[+] ATA: Primary Slave Drive detected");
    if((drive_present[0][1] = identify(0, 1)) == 1)
        kprint("\n[+] ATA: Secondary Master Drive detected");
    if((drive_present[0][0] = identify(0, 0)) == 1)
        kprint("\n[+] ATA: Secondary Slave Drive detected");
}

void ata_clear_buffer()
{
    mem_set(ata_buffer, 0, 512);
}

int ata_drive_select(int primary, int master)
{
    if (!drive_present[primary][master])
        return -1;
    to_select  = (master != is_master);
    is_primary = primary;
    is_master  = master;
    base = primary ? ATA_PRIMARY_BASE : ATA_SECONDARY_BASE;
    return 0;
}

void ata_pio28(uint32_t addr, int sect_count, int to_read)
{
    if (to_select)
        port_w8(base + DEVICE_PORT,
                (is_master ? 0xE0 : 0xF0) | ((addr >> 24) & 0x0F));
    to_select = 0;

    port_w8(base + ERROR_PORT, 0x00);
    port_w8(base + SECTOR_COUNT_PORT, sect_count);
    port_w8(base + LBA_LOW_PORT, (addr & 0xFF));
    port_w8(base + LBA_MID_PORT, (addr >> 8) & 0xFF);
    port_w8(base + LBA_HI_PORT,  (addr >> 16) & 0xFF);

    port_w8(base + COMMAND_PORT, to_read ? 0x20 : 0x30);

    if (!sect_count)
        sect_count = 256;

    /*kprint("\n[+] ATA: PIO28 Initiating Transfer");*/

    while (sect_count--) {
        uint8_t status = port_r8(base + COMMAND_PORT);

        while ((status & 0x80) == 0x80)
            status = port_r8(base + COMMAND_PORT);

        port_r8(base + COMMAND_PORT);   // 400ns delay
        port_r8(base + COMMAND_PORT);
        port_r8(base + COMMAND_PORT);
        port_r8(base + COMMAND_PORT);

        while ((status & 0x08) != 0x08 &&
               (status & 0x01) != 0x01)
            status = port_r8(base + COMMAND_PORT);

        if (status & 0x01) {
            kprint("\n[!] ATA: DRIVE ERROR");
            return;
        }

        for (int i = 0; i < 256; i++) {
            if (to_read)
                ata_buffer[i] = port_r16(base + DATA_PORT);
            else
                port_w16(base + DATA_PORT, ata_buffer[i]);
        }
    }

    /*kprint("\n[+] ATA: PIO28 Success");*/
}

void ata_flush()
{
    uint8_t status;

    if (to_select)
        port_w8(base + DEVICE_PORT, is_master ? 0xE0 : 0xF0);
    to_select = 0;

    port_w8(base + COMMAND_PORT, 0xE7);     // flush command

    status = port_r8(base + COMMAND_PORT);
    while ((status & 0x80) == 0x80)
        status = port_r8(base + COMMAND_PORT);

    if (status & 0x01) {
        kprint("\n[!] ATA: DRIVE ERROR");
        return;
    }
}

void ata_read_sector(uint32_t addr, uint8_t *buf)
{
    ata_pio28(addr, 1, 1);
    mem_cpy(ata_buffer, buf, 512);
}

void ata_write_sector(uint32_t addr, uint8_t *buf)
{
    mem_cpy(buf, ata_buffer, 512);
    ata_pio28(addr, 1, 0);
    ata_flush();
}
