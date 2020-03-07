#include <stdio.h>
#include <stdlib.h>
#include "minfs.h"

int main(int argc, char* argv[])
{
   FILE * disk;
   uint32_t magic;
   size_t b_read;
   struct superblock sb;
   struct partition_table pt;
   disk = fopen(T_PATH, "r");
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

   printf("Superblock Contents:\n");
   printf("ninodes:\t\t%d\n", sb.ninodes);
   printf("i_blocks:\t\t%d\n", sb.i_blocks);
   printf("z_blocks:\t\t%d\n", sb.z_blocks);
   printf("firstdata:\t\t%d\n", sb.firstdata);
   printf("log_zone_size:\t\t%d\n", sb.log_zone_size);
   printf("max_file:\t\t%ld\n", sb.max_file);
   printf("zones:\t\t\t%d\n", sb.zones);
   printf("magic:\t\t\t0x%X\n", sb.magic);
   printf("blocksize:\t\t%d\n", sb.blocksize);
   printf("subversion:\t\t%d\n", sb.subversion);
   printf("\nPT\n");
   printf("0x%X\n", pt.bootind);
   return 0;
}

