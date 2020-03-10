#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include "minfs.h"

uint32_t fs_base = 0;
FILE *disk = NULL;
struct superblock super_block;
int main(int argc, char* argv[])
{
   uint32_t lFirst = -1;
   Options opts = {FALSE, -1, -1, "", "", ""};
   parse_options(argc, argv, TYPE_MINLS, &opts);
   printf("(from minls) SuperBlock location: %d\n", SB_BASE);
   printf("(from minls) Partition Table location: 0x%X\n", PT_BASE);
   disk = fopen(opts.image, "r");
   if(disk == NULL){
      exit(-1);
   }

   lFirst = get_fs(opts.part);
   fs_base = (SECTOR_SIZE * lFirst);
   lFirst = get_fs(opts.subpart);
   return 0;
}
