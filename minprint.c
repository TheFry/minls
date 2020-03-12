#include <stdio.h>
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
