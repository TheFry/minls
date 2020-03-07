#include <stdio.h>
#include <stdlib.h>
#include "minfs.h"

int main(int argc, char* argv[])
{
   FILE * disk;
   uint32_t magic;
   size_t b_read;
   disk = fopen(T_PATH, "r");
   if(fseek(disk, 1024, SEEK_SET))
   {
      perror("fseek");
   }
   b_read = fread(&magic, sizeof(uint32_t), 1, disk);
   printf("Bytes read: %zd\n", b_read);
   printf("Magic #: %d\n", magic);
   return 0;
}

