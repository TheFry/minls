/*
0x1BE location of_the partition_table
0x81 partition_type for Minix
0x55 byte 510 of a boot sector with a valid partition_table
0xAA byte 511 of a boot sector with a valid partition_table
0x4D5A_the minix magic number
0x5A4D minix magic number on a byte-reversed filesystem
64 size of an inode in bytes
64 size of a directory entry in bytes


0170000 File type mask
0100000 Regular file
0040000 Directory
0000400 Owner read permission
0000200 Owner write permission
0000100 Owner execute permission
0000040 Group read permission
0000020 Group write permission
0000010 Group execute permission
0000004 Other read permission
0000002 Other write permission
0000001 Other execute permission
*/
#define MODE_DIR  0040000
#define MODE_REG  0100000
#define MODE_LN   0120000
#define MODE_OR   0000400 
#define MODE_OW   0000200 
#define MODE_OX   0000100
#define MODE_GR   0000040 
#define MODE_GW   0000020 
#define MODE_GX   0000010 
#define MODE_WR   0000004 
#define MODE_WW   0000002 
#define MODE_WX   0000001 

#define SEC_MASK 0x3F

#define T_PATH "./test_fs"
#define SB_OFFSET 1024
#define PT_OFFSET 0x1BE
/* Use this whenever reading the superblock */
#define SB_BASE (fs_base + SB_OFFSET)
/* Use this whenever reading the partition table */
#define PT_BASE (fs_base + PT_OFFSET)
#define SECTOR_SIZE 512
#define STATIC_BLOCKS 2
#define ROOT_INODE 1
#define MINIX_PART_TYPE 0x81
#define PT_MAGIC_NUM 0xAA55
#define MINIX_MAGIC_NUM 0x4D5A
#define INODE_SIZE 64
#define INODE_TABLE_OFFSET (fs_base + (STATIC_BLOCKS + super_block.i_blocks + \
super_block.z_blocks) * super_block.blocksize)
#define ADDRESS_OF_INODE(x) (INODE_TABLE_OFFSET + ((x) - 1) * INODE_SIZE)
#define ZONE_SIZE (super_block.blocksize << super_block.log_zone_size)
#define ADDRESS_OF_ZONE(x) (fs_base + (ZONE_SIZE * (x)))
#define NUM_ZONES_INDR (super_block.blocksize / 32)

#define ZONES_IN_FILE(size) (size % ZONE_SIZE > 0 ? \
(size / ZONE_SIZE) + 1 : size / ZONE_SIZE)

#define DIRENT_SIZE 64
#define PT_MAGIC_NUM_OFFSET 510
#define PT_MAGIC_NUM_LOCATION (fs_base + PT_MAGIC_NUM_OFFSET)
#define PT_ENTRIES 4
#define TRUE 1 
#define FALSE 0
#define TYPE_MINLS 0x10
#define TYPE_MINGET 0x20

#include <stdint.h>
extern uint32_t fs_base;
extern FILE *disk;
extern struct superblock super_block;
struct __attribute__ ((__packed__))superblock
{ 
  /* Minix Version 3 Superblock
   *_this structure found in fs/super.h
   * in minix 3.1.1
   */
   /* on disk._These fields and orientation are non–negotiable */
   uint32_t ninodes; /* number of inodes in_this filesystem */
   uint16_t pad1; /* make_things line up properly */
   int16_t i_blocks; /* # of blocks used by inode bit map */
   int16_t z_blocks; /* # of blocks used by zone bit map */
   uint16_t firstdata; /* number of first data zone */
   int16_t log_zone_size; /* log2 of blocks per zone */
   int16_t pad2; /* make_things line up again */
   uint32_t max_file; /* maximum file size */
   uint32_t zones; /* number of zones on disk */
   int16_t magic; /* magic number */
   int16_t pad3; /* make_things line up again */
   uint16_t blocksize; /* block size in bytes */
   uint8_t subversion; /* filesystem sub–version */
};



/* Minix inode */
#define DIRECT_ZONES 7
struct __attribute__ ((__packed__))inode 
{
   uint16_t mode; /* mode */
   uint16_t links; /* number or links */
   uint16_t uid;
   uint16_t gid;
   uint32_t size;
   int32_t atime;
   int32_t mtime;
   int32_t ctime;
   uint32_t zone[DIRECT_ZONES];
   uint32_t indirect;
   uint32_t two_indirect;
   uint32_t unused;
};


/* Partition table entry */
struct __attribute__ ((__packed__ )) pt_entry
{
   uint8_t bootind;      /* Magic boot number (0x80 if bootable) */
   uint8_t start_head;   /* Start of partition in CHS */
   uint8_t start_sec;
   uint8_t start_cyl;    /* Type of partition, 0x81 if minix */
   uint8_t type;         /* Type of partition, 0x81 if minix */
   uint8_t end_head;     /* End of partition in CHS */
   uint8_t end_sec;
   uint8_t end_cyl;
   uint32_t lFirst;       /* First sector with LBA addressing */
   uint32_t size;         /* size of partition in sectors */
};

struct __attribute__ ((__packed__ )) dirent
{
   uint32_t inumber;
   char name[60];
};

typedef struct opts
{
   short verbose;
   int part;
   int subpart;
   char *srcpath; /* This is where path gets put for minls */
   char *dstpath;
   char *image;
} Options;

void print_usage(char *name, int type);
void parse_options(int argc, char* argv[], int type, Options *opts);
void print_mode(uint16_t mode);
void read_inode(int num, struct inode *data);
void read_zone(uint32_t num, void *buffer);
uint32_t get_part(int pt_num);
void find_fs(Options opts);
int ls(struct inode dir, char *path);
void copy_data(struct inode *, char *);
void load_superblock(Options opts);
uint32_t get_zone_list(struct inode node, uint32_t *buff, uint32_t size);
int traverse_path(char *path, struct inode *file);
int find_file(char *fname, struct inode dir, struct inode *file);
void print_pt();
void print_sb();
void print_inode(struct inode node);
