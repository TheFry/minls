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
   Options opts = {FALSE, -1, -1, NULL, NULL, NULL};
   parse_options(argc, argv, TYPE_MINLS, &opts);
   disk = fopen(opts.image, "r");
   if(disk == NULL){
      exit(-1);
   }
   find_fs(opts);
   load_superblock();
   midnight_toker(opts.srcpath, &dir);
   printf("%s:\n", opts.srcpath == NULL ? "/" : opts.srcpath);
   ls(dir, opts.srcpath);
   return 0;
}
