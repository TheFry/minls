#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "minfs.h"

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

void print_pt()
{
   struct pt_entry pt[PT_ENTRIES];
   uint8_t start_sec, end_sec; 
   uint16_t start_cyl, end_cyl;
   int i;
   
   if(fseek(disk, PT_BASE, SEEK_SET))
   {
      perror("fseek partition (print)");
      exit(EXIT_FAILURE);
   }

   if(fread(pt, sizeof(struct pt_entry), PT_ENTRIES, disk) != PT_ENTRIES)
   {
      perror("fread partition (print)");
      exit(EXIT_FAILURE);
   }
   fprintf(stderr, 
      "       ----Start----      ------End-----\n");
   fprintf(stderr, 
      "  Boot head  sec  cyl Type head  sec  cyl      First       Size\n");
   for(i = 0; i < PT_ENTRIES; i++)
   {
      start_sec = pt[i].start_sec & SEC_MASK;
      end_sec = pt[i].end_sec & SEC_MASK;
      
      /* load the top 2 bits of sector start into a 16 bit buffer so it can 
       * be properly shifted when we or it with start_cyl
       */
      start_cyl = pt[i].start_sec & 0xC0;
      start_cyl = (start_cyl << 2) | pt[i].start_cyl;
      end_cyl = pt[i].end_sec & 0xC0;
      end_cyl = (end_cyl << 2) | pt[i].end_cyl;
      fprintf(stderr, 
         "  0x%02X %4d  %3d  %3d 0x%02X %4d  %3d  %3d  %9d  %9d\n",
         pt[i].bootind, pt[i].start_head, start_sec, start_cyl,
         pt[i].type, pt[i].end_head, end_sec, end_cyl, 
         pt[i].lFirst, pt[i].size);
   }
}

void print_sb()
{
   fprintf(stderr, "\nSuperblock Contents:\n");
   fprintf(stderr, "Stored Fields:\n");
   fprintf(stderr, "  ninodes    %9d\n", super_block.ninodes);
   fprintf(stderr, "  i_blocks   %9d\n", super_block.i_blocks);
   fprintf(stderr, "  z_blocks   %9d\n", super_block.z_blocks);
   fprintf(stderr, "  firstdata   %8d\n", super_block.firstdata);
   fprintf(stderr, "  log_zone_size  %5d (zone size: %d)\n", 
      super_block.i_blocks, ZONE_SIZE);
   fprintf(stderr, "  max_file  %10u\n", super_block.max_file);
   fprintf(stderr, "  magic         0x%04X\n", super_block.magic);
   fprintf(stderr, "  zones      %9d\n", super_block.zones);
   fprintf(stderr, "  blocksize  %9d\n", super_block.blocksize);
   fprintf(stderr, "  subversion  %8d\n", super_block.subversion);
}

void print_inode(struct inode node)
{
   int i;
   time_t atime, cctime, mtime;
   atime = node.atime;
   cctime = node.ctime;
   mtime = node.mtime;
   fprintf(stderr, "\nFile inode:\n");   
   fprintf(stderr, "  uint16_t mode          0x%04X\n", node.mode);
   /*
   print_mode(node.mode);
   fprintf(stderr, ")\n"); */
   fprintf(stderr, "  uint16_t  links     %9d\n", node.links);
   fprintf(stderr, "  uint16_t  uid       %9d\n", node.uid);
   fprintf(stderr, "  uint16_t  gid       %9d\n", node.gid);
   fprintf(stderr, "  uint32_t  size     %10u\n", node.size);
   fprintf(stderr, "  int32_t   atime    %10u --- %s", 
      node.atime, ctime(&atime));
   fprintf(stderr, "  int32_t   mtime    %10u --- %s", 
      node.mtime, ctime(&mtime));
   fprintf(stderr, "  int32_t   ctime    %10u --- %s\n", 
      node.ctime, ctime(&cctime));
   fprintf(stderr, "Direct zones:\n");
   for(i = 0; i < DIRECT_ZONES; i++)
   {
      fprintf(stderr, "            zone[%d]   = %10u\n", i, node.zone[i]);
   }
   fprintf(stderr, " uint32_t  indirect   = %10u\n", node.indirect);  
   fprintf(stderr, " uint32_t  double     = %10u\n\n", node.two_indirect);  
}
