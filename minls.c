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
   struct inode dir;
   Options opts = {FALSE, -1, -1, "", "", ""};
   parse_options(argc, argv, TYPE_MINLS, &opts);
   disk = fopen(opts.image, "r");
   if(disk == NULL){
      exit(-1);
   }
   find_fs(opts);
   load_superblock();
   get_inode(ROOT_INODE, &dir);
   ls(dir);
   return 0;
}
