#include "file.h"
#include "../drivers/ata.h"

int read_file(int fd, char *buf)
{
    ata_read_sector(fd, (uint8_t*)buf);
    return 0;
}

int write_file(int fd, char *buf)
{
    ata_write_sector(fd, (uint8_t*)buf);
    return 0;
}
