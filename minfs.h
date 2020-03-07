/*
0x1BE location of_the partition_table
0x81 partition_type for Minix
0x55 byte 510 of a boot sector with a valid partition_table
0xAA byte 511 of a boot sector with a valid partition_table
0x4D5A_the minix magic number
0x5A4D minix magic number on a byte-reversed filesystem
64 size of an inode in bytes
64 size of a directory entry in bytes
*/


#define T_PATH ./test_fs
#include <stdint.h>
struct superblock
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
struct inode {
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