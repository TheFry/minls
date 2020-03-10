#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include "minfs.h"

uint32_t fs_base = 512;

int main(int argc, char* argv[])
{
   Options opts = {FALSE, -1, -1, "", "", ""};
   parse_options(argc, argv, TYPE_MINLS, &opts);
   printf("(from minls) SuperBlock location: %d\n", SB_BASE);
   printf("(from minls) Partition Table location: 0x%X\n", PT_BASE);
   return 0;
}
