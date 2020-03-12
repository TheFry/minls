#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "minfs.h"

uint32_t fs_base = 0;
FILE *disk = NULL;
struct superblock super_block;
void copy_data(struct inode *, char *);

int main(int argc, char* argv[])
{
   struct inode file;
   Options opts = {FALSE, -1, -1, NULL, NULL, NULL};
   parse_options(argc, argv, TYPE_MINGET, &opts);
   disk = fopen(opts.image, "r");
   if(disk == NULL){
      exit(-1);
   }
   find_fs(opts);
   load_superblock();
   traverse_path(opts.srcpath, &file);
   copy_data(&file, opts.dstpath);
   return 0;
}


void copy_data(struct inode *file, char *outdir)
{
   FILE *outfile = NULL;
   uint32_t *zone_nums = NULL;
   uint8_t *buff = NULL;
   int zone_count = 0;
   int i = 0;

   /* No file provided */
   if(outdir == NULL)
   {
      fprintf(stderr, "No output file specified\n");
      exit(EXIT_FAILURE);
   }


   outfile = fopen(outdir, "w");
   if(outfile == NULL){
      perror(outdir);
   }

   zone_count = ZONES_IN_FILE(file->size);
   zone_nums = malloc(sizeof(uint32_t) * zone_count);
   buff = malloc(sizeof(uint8_t) * ZONE_SIZE);
   
   /* Check all the buffers for validity */
   if(zone_nums == NULL || buff == NULL)
   {
      fprintf(stderr, "Error malloc copy_data\n");
      exit(EXIT_FAILURE);
   }



   /* Check that the file given is a regular file */
   if(file->mode != MODE_REG)
   {
      fprintf(stderr, "Not a regular file\n");
      exit(EXIT_FAILURE);
   }



   get_zone_list(*file, zone_nums, zone_count);

   for(i = 0; i < zone_count; i++)
   {
      read_zone(zone_nums[i], buff);
      if(fwrite(buff, sizeof(uint8_t) * ZONE_SIZE, 1, outfile) <= 0){
         perror(outdir);
      }
      memset(buff, '\0', sizeof(uint8_t) * ZONE_SIZE);
   }
   free(buff);
   free(zone_nums);
   fclose(outfile);
}
