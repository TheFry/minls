#include <stdlib.h>
#include <stdio.h>
#include "minfs.h"


static uint32_t fs_base;
static FS *disk;
static superblock super_block;


int main(int argc, char **argv)
{
   disk = fopen(T_PATH, "r");
   fs_base = 0;
}


/* Search partitons for the right filesystem */
int get_fs(int pt_num, int spt_num)
{
   struct pt_entry pt;
   struct pt_entry spt;
   uint16_t valid_pt = -1;

   /* Get selected primary partition */

   if(fseek(disk, PT_MAGIC_NUM_OFFSET, SEEK_SET))
   {
      perror("fseek primary partiton");
   }

   if(fread(&valid_pt, sizeof(valid_pt), 1, disk))
   {
      perror("fread primary partition");
   }

   if(valid_pt != PT_MAGIC_NUM){
      perror("Not a valid partition table");
      exit(EXIT_FAILURE);
   }

   if(fseek(disk, sizeof(pt_entry) * pt_num, SEEK_SET))                          >
   {
      perror("fseek primary partiton");
   }
   
   if(fread(&pt_entry, sizeof(pt_entry), 1, disk))
   {
      perror("fread primary partition");
   }

   if(pt_entry.type != MINIX_PART_TYPE){
      fprintf(stderr, "Not a valid minix partition\n");
      exit(EXIT_FAILURE);
   }



}