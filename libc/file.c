#include "file.h"
#include "../drivers/ata.h"

int read_file(int fd, char *buf)
{
    if (ata_drive_select(1, 1) < 0)
        return -1;
    ata_read_sector(2*fd, (uint8_t*)buf);
    ata_read_sector(2*fd+1, (uint8_t*)buf+512);
    return 0;
}

int write_file(int fd, char *buf)
{
    if (ata_drive_select(1, 1) < 0)
        return -1;
    ata_write_sector(2*fd, (uint8_t*)buf);
    ata_write_sector(2*fd+1, (uint8_t*)buf+512);
    return 0;
}
