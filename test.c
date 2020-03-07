#include <stdio.h>
#include <stdlib.h>
#include "minfs.h"

int main(int argc, char* argv[])
{
   FILE * disk;
   uint32_t magic;
   uint8_t m1;
   uint8_t m2;
   size_t b_read;
   struct superblock sb;
   struct partition_table pt;
   disk = fopen("//home/pn-cs453/Given/Asgn5/Images/HardDisk", "r");
   if(disk == NULL)
   {
      perror("fopen");
   }
   if(fseek(disk, 1024, SEEK_SET))
   {
      perror("fseek");
   }
   b_read = fread(&sb, sizeof(struct superblock), 1, disk);
   if(fseek(disk, PT_LOCATION, SEEK_SET))
   {
      perror("fseek");
   }

   b_read = fread(&pt, sizeof(struct partition_table), 1, disk);
   if(fseek(disk, 510, SEEK_SET))
   {
      perror("fseek");
   }
   fread(&m1, 1, 1, disk);
   fread(&m2, 1, 1, disk);

   printf("FROM START OF DISK\n");
   printf("GARBAGE Superblock Contents:\n");
   printf("ninodes:\t\t%zd\n", sb.ninodes);
   printf("i_blocks:\t\t%zd\n", sb.i_blocks);
   printf("z_blocks:\t\t%d\n", sb.z_blocks);
   printf("firstdata:\t\t%d\n", sb.firstdata);
   printf("log_zone_size:\t\t%d\n", sb.log_zone_size);
   printf("max_file:\t\t%ld\n", sb.max_file);
   printf("zones:\t\t\t%d\n", sb.zones);
   printf("magic:\t\t\t0x%X\n", sb.magic);
   printf("blocksize:\t\t%d\n", sb.blocksize);
   printf("subversion:\t\t%d\n", sb.subversion);
   printf("\nPT, Partition 0\n");
   printf("Magic #:\t\t0x%X\n", pt.bootind);
   printf("Type:\t\t\t0x%X\n", pt.type);
   printf("lFirst:\t\t\t%zd\n", pt.lFirst);
   printf("size:\t\t\t%zd\n", pt.size);

   printf("\nPartition Signature:\n");
   printf("M1: 0x%X\nM2: 0x%X\n", m1, m2);
   // Read from partition 0
   printf("\n\nPARTITION 0:\n");
   if(fseek(disk, SB_LOCATION + SECTOR_SIZE*18, SEEK_SET))
   {
      perror("fseek");
   }
   b_read = fread(&sb, sizeof(struct superblock), 1, disk);
   printf("Superblock Contents:\n");
   printf("ninodes:\t\t%zd\n", sb.ninodes);
   printf("i_blocks:\t\t%zd\n", sb.i_blocks);
   printf("z_blocks:\t\t%d\n", sb.z_blocks);
   printf("firstdata:\t\t%d\n", sb.firstdata);
   printf("log_zone_size:\t\t%d\n", sb.log_zone_size);
   printf("max_file:\t\t%ld\n", sb.max_file);
   printf("zones:\t\t\t%d\n", sb.zones);
   printf("magic:\t\t\t0x%X\n", sb.magic);
   printf("blocksize:\t\t%d\n", sb.blocksize);
   printf("subversion:\t\t%d\n", sb.subversion);
   if(fseek(disk, PT_LOCATION+SECTOR_SIZE*17, SEEK_SET))
   {
      perror("fseek");
   }
   b_read = fread(&pt, sizeof(struct partition_table), 1, disk);
   printf("\nPT\n");
   printf("Magic #:\t\t0x%X\n", pt.bootind);
   printf("Type:\t\t\t0x%X\n", pt.type);
   printf("lFirst:\t\t\t%zd\n", pt.lFirst);
   printf("size:\t\t\t%zd\n", pt.size);
   if(fseek(disk, 510 + SECTOR_SIZE*17, SEEK_SET))
   {
      perror("fseek");
   }
   fread(&m1, 1, 1, disk);
   fread(&m2, 1, 1, disk);
   printf("\nPartition Signature:\n");
   printf("M1: 0x%X\nM2: 0x%X\n", m1, m2);
   return 0;
}

