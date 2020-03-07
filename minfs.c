#include <stdlib.h>
#include <stdio.h>
#include "minfs.h"
int main(int argc, char **argv)
{
   FILE *fd = fopen(T_PATH, "r");
   printf("%d\n", &fd);
}