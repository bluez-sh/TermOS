#ifndef FS_SFS_H
#define FS_SFS_H

#include "../drivers/ata.h"
#include "../cpu/types.h"

#define SFS_MAGIC   0xf0f03410

// disk size = 4K
#define DISK_SIZE   4096
#define BLOCK_SIZE  512

struct SuperBlock {
    uint32_t magic; 
    uint32_t nb_blocks;
    uint32_t nb_iblocks;
    uint32_t nb_inodes;
} super_block;

#define POINTERS_PER_INODE 5

struct Inode {
    uint32_t valid;     // inode created or not
    uint32_t size;      // logical size of inode data in bytes
    uint32_t direct[POINTERS_PER_INODE]; // direct pointers
    uint32_t indirect;  // indirect pointers
};

#define INODES_PER_BLOCK (BLOCK_SIZE/(sizeof(struct Inode)))
#define POINTERS_PER_BLOCK (BLOCK_SIZE/(sizeof(uint32_t)))

union Block {
    struct SuperBlock super;                // Super block
    struct Inode inodes[INODES_PER_BLOCK];  // Inode block
    uint32_t ptr[POINTERS_PER_BLOCK];       // Pointer block
    char data[BLOCK_SIZE];                  // Data block
};

void sfs_debug();
int sfs_format();
int sfs_mount();

int sfs_create();
int sfs_remove(uint32_t inum);
int sfs_stat(uint32_t inum);
int sfs_read(uint32_t inum, char *data, uint32_t len, uint32_t offset);
int sfs_write(uint32_t inum, char *data, uint32_t len, uint32_t offset);

#endif
