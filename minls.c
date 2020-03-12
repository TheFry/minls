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
   load_superblock(opts);
   traverse_path(opts.srcpath, &dir);
   if(opts.verbose)
   {
      print_inode(dir);
   }
   if(dir.mode & MODE_DIR)
   {
      printf("%s:\n", opts.srcpath == NULL ? "/" : opts.srcpath);
   }
   ls(dir, opts.srcpath);
   fclose(disk);
   return 0;
}


/** Lists all the files in a directory 'dir'
 * If the dir passed is actually just a file it shits the bed
 * In that case print the mode and size then need to print the 
 * srcpath at the end.
 */
int ls(struct inode dir, char *path)
{
   int zones, entries;
   uint32_t *zone_nums;
   int i, d;
   int listed = 0;
   struct dirent *directory;
   struct inode file;
   
   if(!(dir.mode & MODE_DIR))
   {
      print_mode(dir.mode);
      printf(" %9d %s\n", dir.size, path);
      return 0;
   }
   zones = ZONES_IN_FILE(dir.size);
   directory = malloc(ZONE_SIZE);
   zone_nums = malloc(sizeof(uint32_t) * zones);

   entries = dir.size / DIRENT_SIZE;
   get_zone_list(dir, zone_nums, zones);

   for(i = 0; i < zones; i++)
   {
      read_zone(zone_nums[i], directory);
      for(d = 0; d < (ZONE_SIZE / DIRENT_SIZE); d++)
      {
         if(listed >= entries)
         {
            break;
         }
         if(directory[d].inumber != 0)
         {
            read_inode(directory[d].inumber, &file);
            print_mode(file.mode);
            printf(" %9d %s\n", file.size, directory[d].name);
         }
         listed++;
      }
   }
   free(directory);
   free(zone_nums);
   return 0;
} 
