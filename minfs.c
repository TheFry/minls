#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "minfs.h"
#include <string.h>
extern uint32_t fs_base;
extern FILE *disk;
extern struct superblock super_block;




void load_superblock()
{
   fseek(disk, SB_BASE, SEEK_SET);
   fread(&super_block, sizeof(struct superblock), 1, disk);
   if(super_block.magic != MINIX_MAGIC_NUM)
   {
      fprintf(stderr, "Bad magic number. (0x%04X)\n", super_block.magic);
      fprintf(stderr, "This doesn't look like a MINIX filesystem.\n");
      exit(EXIT_FAILURE);
   }
}


void find_fs(Options opts)
{
   uint32_t lFirst;
   if(opts.part >= 0)
   {
      lFirst = get_part(opts.part);
      fs_base = (SECTOR_SIZE * lFirst);
   }
   if(opts.subpart >= 0)
   {
      lFirst = get_part(opts.subpart);
      fs_base = (SECTOR_SIZE * lFirst);
   }
}


/** MAKE SURE fs_base HAS BEEN SET! 
  * This function treat fs_base as the begining of a partition
  * AKA a partition table 
*/
uint32_t get_part(int pt_num)
{
   struct pt_entry pt;
   uint32_t table_offset;
   uint16_t valid_pt;

   table_offset = PT_BASE + (sizeof(struct pt_entry) * pt_num);
   valid_pt = -1;

   /** Check the validity of the PT*/
   if(fseek(disk, PT_MAGIC_NUM_LOCATION, SEEK_SET))
   {
      perror("fseek primary partiton");
      exit(EXIT_FAILURE);
   }

   if(fread(&valid_pt, sizeof(valid_pt), 1, disk) != 1)
   {
      perror("fread primary partition");
      exit(EXIT_FAILURE);
   }

   /* Test them */
   if(valid_pt != PT_MAGIC_NUM){
      fprintf(stderr, "Not a valid partition table\n");
      exit(EXIT_FAILURE);
   }
   
   /** Now get the PT entry, check the fs type */
   if(fseek(disk, table_offset, SEEK_SET))
   {
      perror("fseek primary partiton");
      exit(EXIT_FAILURE);
   }
   
   if(fread(&pt, sizeof(struct pt_entry), 1, disk) != 1)
   {
      perror("fread primary partition");
      exit(EXIT_FAILURE);
   }

   if(pt.type != MINIX_PART_TYPE){

      fprintf(stderr, "Not a valid minix partition\n");
      exit(EXIT_FAILURE);
   }
   /* Return fs start location */
   return pt.lFirst;
}

/** traverse_path:
 * Traverses the path given and returns the inode of the
 * file at the end of the path
 * Makes a local copy of the path so it doesn't mess up the path that gets
 * passed in
 */
void midnight_toker(char *path, struct inode *file)
{
   struct inode dir;
   char *fname;   
   char *tmp_path;
   read_inode(ROOT_INODE, file);
   dir = *file;
   tmp_path = malloc(sizeof(char) * strlen(path));
   strcpy(tmp_path, path);
   fname = strtok(tmp_path, "/");
   while(fname != NULL)
   {
      if(find_file(fname, dir, file) == -1)
      {
         fprintf(stderr, "%s: File not found\n", path);
         exit(EXIT_FAILURE);
      }
      dir = *file;
      fname = strtok(NULL, "/");
   }
}

int find_file(char *fname, struct inode dir, struct inode *file)
{
   int zones, entries;
   uint32_t *zone_nums;
   int i, d;
   int listed = 0;
   struct dirent *directory;
   if(!(dir.mode & MODE_DIR))
   {
      fprintf(stderr, "Not a directory\n");
      return -1;
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
            free(directory);
            free(zone_nums);
            return -1;
         }
         if(directory[d].inumber != 0)
         {
            if(strncmp(fname, directory[d].name, 60) == 0)
            {
               read_inode(directory[d].inumber, file);
               break;
            }
         }
         listed++;
      }
   }
   free(directory);
   free(zone_nums);
   return 1;
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


/** Gets the inode based on a 1-indexed number.
 * Populates 'data' with the inode data
 */
void read_inode(int num, struct inode *data)
{
   int offset = ADDRESS_OF_INODE(num);
   if(fseek(disk, offset, SEEK_SET))
   {
      perror("Fseek read inode");
      exit(EXIT_FAILURE);
   }
   if(fread(data, INODE_SIZE, 1, disk) != 1)
   {
      perror("Fread read inode");
      exit(EXIT_FAILURE);
   }
}


/** Gets the zone based on a 0-indexed number.
 * Populates 'buffer' with the zone data
 */
void read_zone(int num, void *buffer)
{
   int offset = ADDRESS_OF_ZONE(num);
   size_t ret;

   if(num == 0)
   {
      memset(buffer, 0, ZONE_SIZE);
   }
   else
   {
      if(fseek(disk, offset, SEEK_SET))
      {
         perror("Fseek read zone");
         exit(EXIT_FAILURE);
      }
      if((ret = fread(buffer, ZONE_SIZE, 1, disk)) != 1)
      {
         fprintf(stderr, "Ret: %zd\n", ret);
         perror("Fread read zone");
         exit(EXIT_FAILURE);
      }
   }
}


/** Reads all zone numbers in an I-node and combines them into
  * a buffer. The buffer is in the proper order to read a file
  * sequentially.*/
uint32_t get_zone_list(struct inode node, uint32_t *buff, uint32_t size)
{
   int b = 0;
   int c = 0;
   int i = 0;
   uint32_t *zone = NULL;
   uint32_t *double_zone = NULL;

   zone = malloc(ZONE_SIZE);
   double_zone = malloc(ZONE_SIZE);

   while(b < size && i < DIRECT_ZONES)
   {
      buff[b] = node.zone[i];
      ++i;
      ++b;
   }

   i = 0;
   if(b >= size)
   {
      free(zone);
      free(double_zone);
      return b;
   }

   read_zone(node.indirect, zone);
   while(b < size && i < NUM_ZONES_INDR)
   {
      buff[b] = zone[i];
      ++b;
      ++i;
   }

   if(b >= size)
   {
      free(zone);
      free(double_zone);
      return b;
   }

   i = 0;
   read_zone(node.two_indirect, double_zone);
   while(b < size && c < NUM_ZONES_INDR)
   {
      read_zone(double_zone[c], zone);
      while(b < size && i < NUM_ZONES_INDR)
      {
         buff[b] = zone[i];
         ++b;
         ++i;
      }
      i = 0;
      ++c;
   }

   if(b >= size)
   {
      free(zone);
      free(double_zone);
      return b;
   }

   free(zone);
   free(double_zone);
   fprintf(stderr, "Too many zones requested\nRequested: %d\nMax: %d\n",
                    size, DIRECT_ZONES + NUM_ZONES_INDR +
                    (NUM_ZONES_INDR * NUM_ZONES_INDR));
   exit(EXIT_FAILURE);
}


/** Prints the file mode
 * ( it's disgusting I'm sorry :-[ ) -David
 * 
 * Apology not accepted -Luke
 */
void print_mode(uint16_t mode)
{
   if(mode & MODE_DIR) 
   {
      printf("d");
   }
   else
   {
      printf("-");
   }
   if(mode & MODE_OR) 
   {
      printf("r");
   }
   else
   {
      printf("-");
   }
   if(mode & MODE_OW) 
   {
      printf("w");
   }
   else
   {
      printf("-");
   }
   if(mode & MODE_OX) 
   {
      printf("x");
   }
   else
   {
      printf("-");
   }
   if(mode & MODE_GR) 
   {
      printf("r");
   }
   else
   {
      printf("-");
   }
   if(mode & MODE_GW) 
   {
      printf("w");
   }
   else
   {
      printf("-");
   }
   if(mode & MODE_GX) 
   {
      printf("x");
   }
   else
   {
      printf("-");
   }
   if(mode & MODE_WR) 
   {
      printf("r");
   }
   else
   {
      printf("-");
   }
   if(mode & MODE_WW) 
   {
      printf("w");
   }
   else
   {
      printf("-");
   }
   if(mode & MODE_WX) 
   {
      printf("x");
   }
   else
   {
      printf("-");
   }
}


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
   if(opts->subpart >= 0 && opts->part < 0)
   {
      fprintf(stderr, "Cannot have a subpartition without a partition\n");
      print_usage(argv[0], type);
      exit(EXIT_FAILURE);
   }
   fs_base = 0;
}
