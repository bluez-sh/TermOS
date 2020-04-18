#ifndef DRIVERS_ATA_H
#define DRIVERS_ATA_H

#include "../cpu/types.h"
#include "../cpu/ports.h"

#define ATA_PRIMARY_BASE    0x1F0
#define ATA_SECONDARY_BASE  0x170

enum ata_ports {
    DATA_PORT = 0,
    ERROR_PORT,
    SECTOR_COUNT_PORT,
    LBA_LOW_PORT,
    LBA_MID_PORT,
    LBA_HI_PORT,
    DEVICE_PORT,
    COMMAND_PORT,
    CONTROL_PORT = 0x206,   // alternate status
};

void ata_init();
void ata_scan_drives();
void ata_clear_buffer();

int  ata_drive_select(int is_primary, int is_master);
void ata_pio28(uint32_t addr, int sect_count, int to_read);
void ata_flush();

void ata_read_sector(uint32_t addr, uint8_t* buf);
void ata_write_sector(uint32_t addr, uint8_t* buf);
void ata_clear_sector(uint32_t addr);

#endif
