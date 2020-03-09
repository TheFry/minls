#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "minfs.h"

/*
static uint32_t fs_base;
static FS *disk;
static superblock super_block;


int main(int argc, char **argv)
{
   disk = fopen(T_PATH, "r");
   fs_base = 0;
}
*/

/* Search partitons for the right filesystem */
/*
int get_fs(int pt_num, int spt_num)
{
   struct pt_entry pt;
   struct pt_entry spt;
   uint16_t valid_pt = -1;*/

   /* Get selected primary partition 

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
*/
/** Prints usage message. 
 * type is either TYPE_MINLS or TYPE_MINGET
 * name is always argv[0]
 */
void print_usage(char* name, int type)
{
   if(type == TYPE_MINLS)
   {
      fprintf(stderr,
         "usage: %s  [ -v ] [ -p num [ -s num ] ] imagefile [ path ]\n",
            name);
   }
   else if(type == TYPE_MINGET)
   {
      fprintf(stderr,
         "usage: %s  [ -v ] [ -p num [ -s num ] ] imagefile "
         "srcpath [ dstpath ]\n", name);
   }
   fprintf(stderr,"Options:\n");
   fprintf(stderr,
      "\t-p\t part\t --- select partition for filesystem (default: none)\n");
   fprintf(stderr,
      "\t-s\t sub\t --- select subpartition for filesystem (default: none)\n");
   fprintf(stderr,"\t-h\t help\t --- print usage information and exit\n");
   fprintf(stderr,"\t-v\t verbose --- increase verbosity level\n");
}

/** Parse command line options into an options structure.
 * argc is the same argc that is given to main
 * argv is the same argv that is given to main
 * type is either TYPE_MINLS or TYPE_MINGET depending on who is calling this.
 * opts is a pointer to an Options structure to populate
 */
void parse_options(int argc, char* argv[], int type, Options* opts)
{
   char ret;
   char* optstring = "-p:s:v";
   int str_arg = 0;
   if(argc == 1)
   {
      print_usage(argv[0], type);
      exit(EXIT_FAILURE);
   }
   while((ret = getopt(argc, argv, optstring)) != -1)
   {
      if(ret == 'v')
      {
         opts->verbose = TRUE;
      }
      else if(ret == 'p')
      {
         opts->part = (int) strtol(optarg, NULL, 10);
         if(opts->part > 3 || opts->part < 0)
         {
            fprintf(stderr, "Partition %d out of range. Must be 0..3.\n",
               opts->part);
            print_usage(argv[0], type);
            exit(EXIT_FAILURE);
         }  
      }
      else if(ret == 's')
      {
         opts->subpart = (int) strtol(optarg, NULL, 10);
         if(opts->subpart > 3 || opts->subpart < 0)
         {
            fprintf(stderr, "Subpartition %d out of range. Must be 0..3.\n",
               opts->subpart);
            print_usage(argv[0], type);
            exit(EXIT_FAILURE);
         }
      }
      else if(ret == 'h')
      {
         print_usage(argv[0], type);
         exit(EXIT_FAILURE);
      }
      else if(ret == 1)
      {
         if(str_arg == 0)
         {
            opts->image = optarg;
            str_arg++;
         }
         else if(str_arg == 1)
         {
            opts->srcpath = optarg;
            str_arg++;
         }
         else if(str_arg == 2 && type == TYPE_MINGET)
         {
            opts->dstpath = optarg;
            str_arg++;
         }
         else
         {
            fprintf(stderr, "Too many args...\n");
            print_usage(argv[0], type);
            exit(EXIT_FAILURE);
         }
      }
      else
      {
         print_usage(argv[0], type);
         exit(EXIT_FAILURE);
      }
   }
}
