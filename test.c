#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include "minfs.h"

uint32_t fs_base = 0;

void farty_partitions()
{
   FILE * disk;
   uint32_t magic;
   uint8_t m1;
   uint8_t m2;
   uint16_t sig;
   size_t b_read;
   struct superblock sb;
   struct pt_entry pt;
   disk = fopen("//home/pn-cs453/Given/Asgn5/Images/HardDisk", "r");
   if(disk == NULL)
   {
      perror("fopen");
   }
   if(fseek(disk, 1024, SEEK_SET))
   {
      perror("fseek");
   }
   b_read = fread(&sb, sizeof(struct superblock), 1, disk);
   if(fseek(disk, PT_LOCATION, SEEK_SET))
   {
      perror("fseek");
   }

   b_read = fread(&pt, sizeof(struct pt_entry), 1, disk);
   if(fseek(disk, 510, SEEK_SET))
   {
      perror("fseek");
   }
   fread(&m1, 1, 1, disk);
   fread(&m2, 1, 1, disk);

   printf("FROM START OF DISK\n");
   printf("GARBAGE Superblock Contents:\n");
   printf("ninodes:\t\t%zd\n", sb.ninodes);
   printf("i_blocks:\t\t%zd\n", sb.i_blocks);
   printf("z_blocks:\t\t%d\n", sb.z_blocks);
   printf("firstdata:\t\t%d\n", sb.firstdata);
   printf("log_zone_size:\t\t%d\n", sb.log_zone_size);
   printf("max_file:\t\t%ld\n", sb.max_file);
   printf("zones:\t\t\t%d\n", sb.zones);
   printf("magic:\t\t\t0x%X\n", sb.magic);
   printf("blocksize:\t\t%d\n", sb.blocksize);
   printf("subversion:\t\t%d\n", sb.subversion);
   printf("\nPT, Partition 0\n");
   printf("Magic #:\t\t0x%X\n", pt.bootind);
   printf("Type:\t\t\t0x%X\n", pt.type);
   printf("lFirst:\t\t\t%zd\n", pt.lFirst);
   printf("size:\t\t\t%zd\n", pt.size);

   printf("\nPartition Signature:\n");
   printf("M1: 0x%X\nM2: 0x%X\n", m1, m2);
   // Read from partition 0
   printf("\n\nPARTITION 0:\n");
   if(fseek(disk, PT_LOCATION+SECTOR_SIZE*17, SEEK_SET))
   {
      perror("fseek");
   }
   b_read = fread(&pt, sizeof(struct pt_entry), 1, disk);
   printf("\nP0 S0 \n");
   printf("Magic #:\t\t0x%X\n", pt.bootind);
   printf("Type:\t\t\t0x%X\n", pt.type);
   printf("lFirst:\t\t\t%zd\n", pt.lFirst);
   printf("size:\t\t\t%zd\n", pt.size);
   b_read = fread(&pt, sizeof(struct pt_entry), 1, disk);
   printf("\nP0 S1\n");
   printf("Magic #:\t\t0x%X\n", pt.bootind);
   printf("Type:\t\t\t0x%X\n", pt.type);
   printf("lFirst:\t\t\t%zd\n", pt.lFirst);
   printf("size:\t\t\t%zd\n", pt.size);

   b_read = fread(&pt, sizeof(struct pt_entry), 1, disk);
   printf("\nP0 S2\n");
   printf("Magic #:\t\t0x%X\n", pt.bootind);
   printf("Type:\t\t\t0x%X\n", pt.type);
   printf("lFirst:\t\t\t%zd\n", pt.lFirst);
   printf("size:\t\t\t%zd\n", pt.size);
   if(fseek(disk, SB_LOCATION + SECTOR_SIZE*18, SEEK_SET))
   {
      perror("fseek");
   }

   b_read = fread(&sb, sizeof(struct superblock), 1, disk);
   printf("\nSuperblock Contents of Subpart 0 (sector 18):\n");
   printf("ninodes:\t\t%zd\n", sb.ninodes);
   printf("i_blocks:\t\t%zd\n", sb.i_blocks);
   printf("z_blocks:\t\t%d\n", sb.z_blocks);
   printf("firstdata:\t\t%d\n", sb.firstdata);
   printf("log_zone_size:\t\t%d\n", sb.log_zone_size);
   printf("max_file:\t\t%ld\n", sb.max_file);
   printf("zones:\t\t\t%d\n", sb.zones);
   printf("magic:\t\t\t0x%X\n", sb.magic);
   printf("blocksize:\t\t%d\n", sb.blocksize);
   printf("subversion:\t\t%d\n", sb.subversion);
   if(fseek(disk, SB_LOCATION + SECTOR_SIZE*2898, SEEK_SET))
   {
      perror("fseek");
   }
   b_read = fread(&sb, sizeof(struct superblock), 1, disk);
   printf("\nSuperblock Contents of Subpart 2 (sector 2898):\n");
   printf("ninodes:\t\t%zd\n", sb.ninodes);
   printf("i_blocks:\t\t%zd\n", sb.i_blocks);
   printf("z_blocks:\t\t%d\n", sb.z_blocks);
   printf("firstdata:\t\t%d\n", sb.firstdata);
   printf("log_zone_size:\t\t%d\n", sb.log_zone_size);
   printf("max_file:\t\t%ld\n", sb.max_file);
   printf("zones:\t\t\t%d\n", sb.zones);
   printf("magic:\t\t\t0x%X\n", sb.magic);
   printf("blocksize:\t\t%d\n", sb.blocksize);
   printf("subversion:\t\t%d\n", sb.subversion);
   if(fseek(disk, 510 + SECTOR_SIZE*17, SEEK_SET))
   {
      perror("fseek");
   }
   fread(&m1, 1, 1, disk);
   fread(&m2, 1, 1, disk);
   printf("\nPartition Signature:\n");
   printf("M1: 0x%X\nM2: 0x%X\n", m1, m2);
   fseek(disk, 510 + SECTOR_SIZE*17, SEEK_SET);
   fread(&sig, sizeof(uint16_t), 1, disk);
   printf("Signature: 0x%X\n", sig);

}

void peek_fs()
{
   FILE * disk;
   uint32_t magic;
   size_t b_read;
   struct superblock sb;
   struct inode root;
   struct dirent file;
   size_t offset;
   size_t zone_offset;
   disk = fopen(T_PATH, "r");
   if(disk == NULL)
   {
      perror("fopen");
   }
   if(fseek(disk, SB_LOCATION, SEEK_SET))
   {
      perror("fseek");
   }
   b_read = fread(&sb, sizeof(struct superblock), 1, disk);

   printf("FROM START OF DISK\n");
   printf("Superblock Contents:\n");
   printf("ninodes:\t\t%zd\n", sb.ninodes);
   printf("i_blocks:\t\t%zd\n", sb.i_blocks);
   printf("z_blocks:\t\t%d\n", sb.z_blocks);
   printf("firstdata:\t\t%d\n", sb.firstdata);
   printf("log_zone_size:\t\t%d\n", sb.log_zone_size);
   printf("max_file:\t\t%ld\n", sb.max_file);
   printf("zones:\t\t\t%d\n", sb.zones);
   printf("magic:\t\t\t0x%X\n", sb.magic);
   printf("blocksize:\t\t%d\n", sb.blocksize);
   printf("subversion:\t\t%d\n", sb.subversion);

   offset = (2 + sb.i_blocks + sb.z_blocks) * sb.blocksize;
   fseek(disk, offset, SEEK_SET);
   fread(&root, sizeof(struct inode), 1, disk);

   printf("Root Inode:\n");
   printf("Mode:\t\t0x%X\n", root.mode);
   printf("Links:\t\t%d\n", root.links);
   printf("size:\t\t%d\n", root.size);

   printf("Zone[0]:\t%d\n", root.zone[0]);
   
   zone_offset = sb.firstdata * (sb.blocksize << sb.log_zone_size);
   fseek(disk, zone_offset, SEEK_SET);
   fread(&file, sizeof(struct dirent), 1, disk);
   printf("%s's inode: %d\n", file.name, file.inumber);
   fread(&file, sizeof(struct dirent), 1, disk);
   printf("%s's inode: %d\n", file.name, file.inumber);
   fread(&file, sizeof(struct dirent), 1, disk);
   printf("%s's inode: %d\n", file.name, file.inumber);
   fread(&file, sizeof(struct dirent), 1, disk);
   printf("%s's inode: %d\n", file.name, file.inumber);
   fread(&file, sizeof(struct dirent), 1, disk);
   printf("%s's inode: %d\n", file.name, file.inumber);
   fread(&file, sizeof(struct dirent), 1, disk);
   printf("%s's inode: %d\n", file.name, file.inumber);
   fread(&file, sizeof(struct dirent), 1, disk);
   printf("%s's inode: %d\n", file.name, file.inumber);
}

Options opts = {FALSE, -1, -1, ""};
int parse_options(int argc, char* argv[])
{
   char ret;
   char* optstring = "p:s:v";
   if(argc == 1)
   {
      fprintf(stderr,
         "usage: %s  [ -v ] [ -p num [ -s num ] ] imagefile [ path ]\n",
            argv[0]);
      fprintf(stderr,"Options:\n");
      fprintf(stderr,
         "\t-p\t part\t --- select partition for filesystem (default: none)\n");
      fprintf(stderr,
         "\t-s\t sub\t --- select subpartition for filesystem (default: none)\n");
      fprintf(stderr,"\t-h\t help\t --- print usage information and exit\n");
      fprintf(stderr,"\t-v\t verbose --- increase verbosity level\n");
      return FALSE;
   }
   while((ret = getopt(argc, argv, optstring)) != -1)
   {
      if(ret == 'v')
      {
         opts.verbose = TRUE;
      }
      else if(ret == 'p')
      {
         opts.part = (int) strtol(optarg, NULL, 10);
      }
      else if(ret == 's')
      {
         opts.subpart = (int) strtol(optarg, NULL, 10);
      }
      
   }
   printf("Verbose: %d\n", opts.verbose);
   printf("Part: %d\n", opts.part);
   printf("SubPart: %d\n", opts.subpart);
}

int main(int argc, char* argv[])
{
   parse_options(argc, argv);
   return 0;
}

