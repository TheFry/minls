#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include "minfs.h"

uint32_t fs_base = 0;

int main(int argc, char* argv[])
{
   Options opts;
   parse_options(argc, argv, TYPE_MINLS, &opts);
   return 0;
}
