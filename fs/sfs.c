#include "sfs.h"
#include "../drivers/screen.h"
#include "../libc/mem.h"
#include "../libc/string.h"

#define NB_BLOCKS   (DISK_SIZE / BLOCK_SIZE)
#define NB_IBLOCKS  (NB_BLOCKS / 10)
#define NB_INODES   (NB_IBLOCKS * INODES_PER_BLOCK)

static struct Inode inode_table[NB_INODES];

/*
 * Free Blocks Bitmap
 * 0 - free block
 * 1 - used block
 */
static uint8_t free_blocks[NB_BLOCKS];

static void set_free_blocks(struct Inode *inode, int used)
{
    for (unsigned k = 0; k < POINTERS_PER_INODE; k++)
        free_blocks[inode->direct[k]] = used;
    if (inode->indirect) {
        union Block indirect;
        ata_read_sector(inode->indirect, (uint8_t*)&indirect);
        for (unsigned p = 0; p < POINTERS_PER_BLOCK; p++)
            free_blocks[indirect.ptr[p]] = used;
    }
}

static void load_inode(uint32_t inum, struct Inode *inode)
{
    union Block block;
    uint32_t block_num = (inum / INODES_PER_BLOCK) + 1;
    uint32_t index = (inum % INODES_PER_BLOCK);
    ata_read_sector(block_num, (uint8_t*)&block.data);
    mem_cpy(&block.inodes[index], inode, sizeof(struct Inode));
}

static void store_inode(uint32_t inum, struct Inode *inode)
{
    union Block block;
    uint32_t block_num = (inum / INODES_PER_BLOCK) + 1;
    uint32_t index = (inum % INODES_PER_BLOCK);
    ata_read_sector(block_num, (uint8_t*)&block.data);
    mem_cpy(inode, &block.inodes[index], sizeof(struct Inode));
    ata_write_sector(block_num, (uint8_t*)&block.data);
}

void sfs_debug()
{
    kprint("\n[+] SFS: Debugging...");

    ata_read_sector(0, (uint8_t*)&super_block);

    if (super_block.magic != SFS_MAGIC) {
        kprint("\n[!] SFS: Invalid magic");
        kprint("\n[+] SFS: Exiting debug...");
        return;
    }

    char num[16];
    kprint("\nSuperBlock:");
    kprint("\n  Valid Magic number");
    kprint("\n  Total Blocks: ");
    int_to_ascii(super_block.nb_blocks, num);
    kprint(num);
    kprint("\n  Inode Blocks: ");
    int_to_ascii(super_block.nb_iblocks, num);
    kprint(num);
    kprint("\n  Inodes: ");
    int_to_ascii(super_block.nb_inodes, num);
    kprint(num);

    for (uint32_t i = 0; i < super_block.nb_inodes; i++) {
        if (inode_table[i].valid) {
            kprint("\nInode ");
            int_to_ascii(i, num);
            kprint(num);
            kprint(":");
            kprint("\n  size: ");
            int_to_ascii(inode_table[i].size, num);
            kprint(num);
            kprint(" bytes");
            kprint("\n  direct blocks: ");
            int c;
            for (c = 0; c < POINTERS_PER_INODE; c++)
                if (!inode_table[i].direct[c])
                    break;
            int_to_ascii(c, num);
            kprint(num);
        }
    }
}

int sfs_format()
{
    union Block block;
    block.super.magic = SFS_MAGIC;
    block.super.nb_blocks  = NB_BLOCKS;
    block.super.nb_iblocks = NB_IBLOCKS;
    block.super.nb_inodes  = NB_INODES;
    ata_write_sector(0, (uint8_t*)&block);

    mem_set(inode_table, 0, sizeof(inode_table));
    mem_set(block.data, 0, sizeof(block));
    for (unsigned i = 1; i <= NB_IBLOCKS; i++)
        ata_write_sector(i, (uint8_t*)&block);
    return 0;
}

int sfs_mount()
{
    ata_read_sector(0, (uint8_t*)&super_block);
    if (super_block.magic != SFS_MAGIC)
        return -1;

    mem_set(free_blocks, 0, super_block.nb_blocks);

    free_blocks[0] = 1;
    union Block block;
    uint32_t inum = 0;

    for (unsigned i = 1; i <= super_block.nb_iblocks; i++) {
        free_blocks[i] = 1;
        ata_read_sector(i, (uint8_t*)&block.data);

        for (unsigned j = 0; j < INODES_PER_BLOCK; j++, inum++) {
            if (block.inodes[j].valid) {
                // init inode table
                mem_cpy(&block.inodes[j], &inode_table[inum], sizeof(struct Inode));
                // map free blocks
                set_free_blocks(&block.inodes[j], 1);
            }
        }
    }
    return 0;
}

int sfs_create()
{
    for (unsigned i = 0; i < super_block.nb_inodes; i++)
        if (!inode_table[i].valid) {
            inode_table[i].valid = 1;
            store_inode(i, &inode_table[i]);
            return i;
        }
    return -1;
}

int sfs_remove(uint32_t inum)
{
    if (inum > super_block.nb_inodes)
        return -1;
    set_free_blocks(&inode_table[inum], 0);
    mem_set(&inode_table[inum], 0, sizeof(struct Inode));
    store_inode(inum, &inode_table[inum]);
    return 0;
}

int sfs_stat(uint32_t inum)
{
    if (inum > super_block.nb_inodes)
        return -1;
    if (!inode_table[inum].valid)
        return -1;
    return inode_table[inum].size;
}

int sfs_read(uint32_t inum, char *data, uint32_t len, uint32_t offset)
{
    if (inum > super_block.nb_inodes || !data)
        return -1;

    struct Inode inode = inode_table[inum];
    if (!inode.valid)
        return -1;

    union Block block;
    uint32_t next_block   = offset / BLOCK_SIZE;
    uint32_t offset_into  = offset % BLOCK_SIZE;

    unsigned bytes_read = 0;
    while (bytes_read != len) {
        if (offset_into >= BLOCK_SIZE || !bytes_read) {
            if (next_block >= POINTERS_PER_INODE)
                break;
            if (bytes_read) offset_into = 0;
            if (!inode.direct[next_block])
                goto end;
            ata_read_sector(inode.direct[next_block], (uint8_t*)&block.data);
            next_block++;
        }
        data[bytes_read++] = block.data[offset_into++];
    }

    if (bytes_read == len) goto end;

    union Block indirect;
    ata_read_sector(inode.indirect, (uint8_t*)&indirect.data);

    while (bytes_read != len) {
        if (offset_into >= BLOCK_SIZE || !bytes_read) {
            if (next_block >= POINTERS_PER_INODE + POINTERS_PER_BLOCK)
                break;
            if (bytes_read) offset_into = 0;
            if (!indirect.ptr[next_block])
                goto end;
            ata_read_sector(indirect.ptr[next_block - POINTERS_PER_INODE],
                    (uint8_t*)&block.data);
            next_block++;
        }
        data[bytes_read++] = block.data[offset_into++];
    }

end:
    return bytes_read;
}
