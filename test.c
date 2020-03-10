#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include "minfs.h"

uint32_t fs_base = 0;
struct superblock super_block;

void farty_partitions()
{
   FILE * disk;
   uint32_t magic;
   uint8_t m1;
   uint8_t m2;
   uint16_t sig;
   size_t b_read;
   struct superblock super_block;
   struct pt_entry pt;
   disk = fopen("//home/pn-cs453/Given/Asgn5/Images/HardDisk", "r");
   if(disk == NULL)
   {
      perror("fopen");
   }
   if(fseek(disk, 1024, SEEK_SET))
   {
      perror("fseek");
   }
   b_read = fread(&super_block, sizeof(struct superblock), 1, disk);
   if(fseek(disk, PT_BASE, SEEK_SET))
   {
      perror("fseek");
   }

   b_read = fread(&pt, sizeof(struct pt_entry), 1, disk);
   if(fseek(disk, 510, SEEK_SET))
   {
      perror("fseek");
   }
   fread(&m1, 1, 1, disk);
   fread(&m2, 1, 1, disk);

   printf("FROM START OF DISK\n");
   printf("GARBAGE Superblock Contents:\n");
   printf("ninodes:\t\t%zd\n", super_block.ninodes);
   printf("i_blocks:\t\t%zd\n", super_block.i_blocks);
   printf("z_blocks:\t\t%d\n", super_block.z_blocks);
   printf("firstdata:\t\t%d\n", super_block.firstdata);
   printf("log_zone_size:\t\t%d\n", super_block.log_zone_size);
   printf("max_file:\t\t%ld\n", super_block.max_file);
   printf("zones:\t\t\t%d\n", super_block.zones);
   printf("magic:\t\t\t0x%X\n", super_block.magic);
   printf("blocksize:\t\t%d\n", super_block.blocksize);
   printf("subversion:\t\t%d\n", super_block.subversion);
   printf("\nPT, Partition 0\n");
   printf("Magic #:\t\t0x%X\n", pt.bootind);
   printf("Type:\t\t\t0x%X\n", pt.type);
   printf("lFirst:\t\t\t%zd\n", pt.lFirst);
   printf("size:\t\t\t%zd\n", pt.size);

   printf("\nPartition Signature:\n");
   printf("M1: 0x%X\nM2: 0x%X\n", m1, m2);
   // Read from partition 0
   printf("\n\nPARTITION 0:\n");
   if(fseek(disk, PT_BASE+SECTOR_SIZE*17, SEEK_SET))
   {
      perror("fseek");
   }
   b_read = fread(&pt, sizeof(struct pt_entry), 1, disk);
   printf("\nP0 S0 \n");
   printf("Magic #:\t\t0x%X\n", pt.bootind);
   printf("Type:\t\t\t0x%X\n", pt.type);
   printf("lFirst:\t\t\t%zd\n", pt.lFirst);
   printf("size:\t\t\t%zd\n", pt.size);
   b_read = fread(&pt, sizeof(struct pt_entry), 1, disk);
   printf("\nP0 S1\n");
   printf("Magic #:\t\t0x%X\n", pt.bootind);
   printf("Type:\t\t\t0x%X\n", pt.type);
   printf("lFirst:\t\t\t%zd\n", pt.lFirst);
   printf("size:\t\t\t%zd\n", pt.size);

   b_read = fread(&pt, sizeof(struct pt_entry), 1, disk);
   printf("\nP0 S2\n");
   printf("Magic #:\t\t0x%X\n", pt.bootind);
   printf("Type:\t\t\t0x%X\n", pt.type);
   printf("lFirst:\t\t\t%zd\n", pt.lFirst);
   printf("size:\t\t\t%zd\n", pt.size);
   if(fseek(disk, SB_BASE + SECTOR_SIZE*18, SEEK_SET))
   {
      perror("fseek");
   }

   b_read = fread(&super_block, sizeof(struct superblock), 1, disk);
   printf("\nSuperblock Contents of Subpart 0 (sector 18):\n");
   printf("ninodes:\t\t%zd\n", super_block.ninodes);
   printf("i_blocks:\t\t%zd\n", super_block.i_blocks);
   printf("z_blocks:\t\t%d\n", super_block.z_blocks);
   printf("firstdata:\t\t%d\n", super_block.firstdata);
   printf("log_zone_size:\t\t%d\n", super_block.log_zone_size);
   printf("max_file:\t\t%ld\n", super_block.max_file);
   printf("zones:\t\t\t%d\n", super_block.zones);
   printf("magic:\t\t\t0x%X\n", super_block.magic);
   printf("blocksize:\t\t%d\n", super_block.blocksize);
   printf("subversion:\t\t%d\n", super_block.subversion);
   if(fseek(disk, SB_BASE + SECTOR_SIZE*2898, SEEK_SET))
   {
      perror("fseek");
   }
   b_read = fread(&super_block, sizeof(struct superblock), 1, disk);
   printf("\nSuperblock Contents of Subpart 2 (sector 2898):\n");
   printf("ninodes:\t\t%zd\n", super_block.ninodes);
   printf("i_blocks:\t\t%zd\n", super_block.i_blocks);
   printf("z_blocks:\t\t%d\n", super_block.z_blocks);
   printf("firstdata:\t\t%d\n", super_block.firstdata);
   printf("log_zone_size:\t\t%d\n", super_block.log_zone_size);
   printf("max_file:\t\t%ld\n", super_block.max_file);
   printf("zones:\t\t\t%d\n", super_block.zones);
   printf("magic:\t\t\t0x%X\n", super_block.magic);
   printf("blocksize:\t\t%d\n", super_block.blocksize);
   printf("subversion:\t\t%d\n", super_block.subversion);
   if(fseek(disk, 510 + SECTOR_SIZE*17, SEEK_SET))
   {
      perror("fseek");
   }
   fread(&m1, 1, 1, disk);
   fread(&m2, 1, 1, disk);
   printf("\nPartition Signature:\n");
   printf("M1: 0x%X\nM2: 0x%X\n", m1, m2);
   fseek(disk, 510 + SECTOR_SIZE*17, SEEK_SET);
   fread(&sig, sizeof(uint16_t), 1, disk);
   printf("Signature: 0x%X\n", sig);

}

void get_inode(int num, FILE* disk, struct inode* data)
{
   int offset = ADDRESS_OF_INODE(num);
   fseek(disk, offset, SEEK_SET);
   fread(data, INODE_SIZE, 1, disk);
}

void print_mode(uint16_t mode)
{
   if(mode & MODE_DIR) 
   {
      printf("d");
   }
   else
   {
      printf("-");
   }
   if(mode & MODE_OR) 
   {
      printf("r");
   }
   else
   {
      printf("-");
   }
   if(mode & MODE_OW) 
   {
      printf("w");
   }
   else
   {
      printf("-");
   }
   if(mode & MODE_OX) 
   {
      printf("x");
   }
   else
   {
      printf("-");
   }
   if(mode & MODE_GR) 
   {
      printf("r");
   }
   else
   {
      printf("-");
   }
   if(mode & MODE_GW) 
   {
      printf("w");
   }
   else
   {
      printf("-");
   }
   if(mode & MODE_GX) 
   {
      printf("x");
   }
   else
   {
      printf("-");
   }
   if(mode & MODE_WR) 
   {
      printf("r");
   }
   else
   {
      printf("-");
   }
   if(mode & MODE_WW) 
   {
      printf("w");
   }
   else
   {
      printf("-");
   }
   if(mode & MODE_WX) 
   {
      printf("x");
   }
   else
   {
      printf("-");
   }
}

void ls(struct inode dir)
{
   int entries;
   entries = dir.size / DIRENT_SIZE

void peek_fs()
{
   FILE * disk;
   uint32_t magic;
   size_t b_read;
   struct inode root;
   struct inode dir;
   struct dirent file;
   size_t offset;
   size_t zone_offset;
   disk = fopen(T_PATH, "r");
   if(disk == NULL)
   {
      perror("fopen");
   }
   if(fseek(disk, SB_BASE, SEEK_SET))
   {
      perror("fseek");
   }
   b_read = fread(&super_block, sizeof(struct superblock), 1, disk);

   printf("FROM START OF DISK\n");
   printf("Superblock Contents:\n");
   printf("ninodes:\t\t%zd\n", super_block.ninodes);
   printf("i_blocks:\t\t%zd\n", super_block.i_blocks);
   printf("z_blocks:\t\t%d\n", super_block.z_blocks);
   printf("firstdata:\t\t%d\n", super_block.firstdata);
   printf("log_zone_size:\t\t%d\n", super_block.log_zone_size);
   printf("max_file:\t\t%ld\n", super_block.max_file);
   printf("zones:\t\t\t%d\n", super_block.zones);
   printf("magic:\t\t\t0x%X\n", super_block.magic);
   printf("blocksize:\t\t%d\n", super_block.blocksize);
   printf("subversion:\t\t%d\n", super_block.subversion);

   get_inode(ROOT_INODE, disk, &root);

   printf("Root Inode:\n");
   printf("Mode:\t");
   print_mode(root.mode);
   printf("\nLinks:\t\t%d\n", root.links);
   printf("size:\t\t%d\n", root.size);

   printf("Zone[0]:\t%d\n", root.zone[0]);
   
   zone_offset = ADDRESS_OF_ZONE(root.zone[0]);
   fseek(disk, zone_offset, SEEK_SET);
   fread(&file, sizeof(struct dirent), 1, disk);
   printf("%s's inode: %d\n", file.name, file.inumber);
   fread(&file, sizeof(struct dirent), 1, disk);
   printf("%s's inode: %d\n", file.name, file.inumber);
   fread(&file, sizeof(struct dirent), 1, disk);
   printf("%s's inode: %d\n", file.name, file.inumber);
   fread(&file, sizeof(struct dirent), 1, disk);
   printf("%s's inode: %d\n", file.name, file.inumber);
   fread(&file, sizeof(struct dirent), 1, disk);
   printf("%s's inode: %d\n", file.name, file.inumber);
   fread(&file, sizeof(struct dirent), 1, disk);
   printf("%s's inode: %d\n", file.name, file.inumber);

   get_inode(4, disk, &dir);
   printf("src Inode:\n");
   printf("Mode:\t");
   print_mode(dir.mode);
   printf("\nLinks:\t\t%d\n", dir.links);
   printf("size:\t\t%d\n", dir.size);
   printf("Zone[0]:\t%d\n", dir.zone[0]);

   zone_offset = ADDRESS_OF_ZONE(dir.zone[0]);
   printf("Zone offset: %d\n", zone_offset);
   fseek(disk, ADDRESS_OF_ZONE(dir.zone[0]), SEEK_SET);
   fread(&file, sizeof(struct dirent), 1, disk);
   printf("%s's inode: %d\n", file.name, file.inumber);
   fread(&file, sizeof(struct dirent), 1, disk);
   printf("%s's inode: %d\n", file.name, file.inumber);
   fread(&file, sizeof(struct dirent), 1, disk);
   printf("%s's inode: %d\n", file.name, file.inumber);
}


int main(int argc, char* argv[])
{
   peek_fs();
   return 0;
}
