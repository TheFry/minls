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
