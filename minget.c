#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "minfs.h"

uint32_t fs_base = 0;
FILE *disk = NULL;
struct superblock super_block;


int main(int argc, char* argv[])
{
   struct inode file;
   Options opts = {FALSE, -1, -1, NULL, NULL, NULL};
   parse_options(argc, argv, TYPE_MINGET, &opts);
   disk = fopen(opts.image, "r");
   if(disk == NULL){
      perror(opts.image);
      exit(-1);
   }
   
   find_fs(opts);
   load_superblock(opts);
   traverse_path(opts.srcpath, &file);
   if(opts.verbose)
   {
      print_inode(file);
   }
   copy_data(&file, opts.dstpath);
   return 0;
}


void copy_data(struct inode *file, char *outdir)
{
   FILE *outfile = NULL;
   uint32_t *zone_nums = NULL;
   void *buff = NULL;
   int zone_count = 0;
   int i = 0;
   size_t size, write_bytes;
   char *debug_str;
   size = file->size;

   debug_str = malloc(sizeof(char) * 20);
   /* No file provided */
   if(outdir == NULL)
   {
      outfile = stdout;
   }
   else
   {
      outfile = fopen(outdir, "w");     
   }


   if(outfile == NULL){
      perror(outdir);
   }

   zone_count = ZONES_IN_FILE(file->size);
   zone_nums = malloc(sizeof(uint32_t) * zone_count);
   buff = malloc(ZONE_SIZE);
   
   /* Check all the buffers for validity */
   if(zone_nums == NULL || buff == NULL)
   {
      fprintf(stderr, "Error malloc copy_data\n");
      exit(EXIT_FAILURE);
   }



   /* Check that the file given is a regular file */
   if(!(file->mode & MODE_REG) || (file->mode & MODE_LN))
   {
      fprintf(stderr, "Not a regular file\n");
      exit(EXIT_FAILURE);
   }


   /* Get a list of the zone numbers in order */
   get_zone_list(*file, zone_nums, zone_count);

   /* Go through zone by zone and copy */
   for(i = 0; i < zone_count; i++)
   {
      read_zone(zone_nums[i], buff);
      write_bytes = size >= ZONE_SIZE ? ZONE_SIZE : size;
      if(i == DIRECT_ZONES)
      {
         fwrite("End of Direct. INDIRECT BELOW", sizeof(char) * 29, 1, outfile);
      }
      if(i == (NUM_ZONES_INDR + DIRECT_ZONES))
      {
         fwrite("End of Indirect. DOUBLE BELOW", sizeof(char) * 29, 1, outfile);
      }
      if(fwrite(buff, 1, write_bytes, outfile) <= 0){
         perror(outdir);
      }
      sprintf(debug_str, "ZONE(%d)", i);
      fwrite(debug_str, strlen(debug_str), 1, outfile);
      memset(buff, 0, ZONE_SIZE);
      size -= ZONE_SIZE;
   }
   free(buff);
   free(zone_nums);
   fclose(outfile);
}
