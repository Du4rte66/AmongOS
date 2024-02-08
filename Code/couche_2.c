/**
 * @file couche_2_romeo.c
 * @brief contient code de la couche 2
 * @author romeo,duarte,chedli,elyes
 * @date 03/2022
 */

#include <stdio.h>

#include "../Utils/utils.h"
#include "couche_1.h"
#include "couche_2.h"

void write_super_block() {
  write_block(&disk.super_block,0,sizeof(struct super_block_s));
}

void read_super_block() {
  read_block(&disk.super_block,0,sizeof(struct super_block_s));
}

void update_first_free_byte(int newpos) {
  disk.super_block.first_free_byte=newpos;
}

void read_inodes_table(){
  read_block(&disk.inodes,INODES_START,sizeof(inode_table_t));
}

void write_inodes_table(){
  write_block(&disk.inodes,INODES_START,sizeof(inode_table_t));
}

void reculer_inode(int i) {
  // copy inode i on i-1
  disk.inodes[i-1]=disk.inodes[i];

  //deleting inode i
  disk.inodes[i].filename[0]    = '\0';
  disk.inodes[i].size           = 0;
  disk.inodes[i].uid            = 0;
  disk.inodes[i].uright         = 0;
  disk.inodes[i].oright         = 0;
  disk.inodes[i].ctimestamp[0]  = '\0';
  disk.inodes[i].mtimestamp[0]  = '\0';
  disk.inodes[i].nblock         = 0;
  disk.inodes[i].first_byte     = 0;
}

void delete_inode(int index){
  //update super block
  disk.super_block.number_of_files--;
  disk.super_block.nb_blocks_used-=disk.inodes[index].nblock;

  //deleting inode
  disk.inodes[index].filename[0]    = '\0';
  disk.inodes[index].size           = 0;
  disk.inodes[index].uid            = 0;
  disk.inodes[index].uright         = 0;
  disk.inodes[index].oright         = 0;
  disk.inodes[index].ctimestamp[0]  = '\0';
  disk.inodes[index].mtimestamp[0]  = '\0';
  disk.inodes[index].nblock         = 0;
  disk.inodes[index].first_byte     = 0;

  //compacting table
  int i=index+1;
  while (i<INODE_TABLE_SIZE && disk.inodes[i].first_byte!=0) {
    reculer_inode(i);
    i++;
  }
}

int get_unused_inode(){
  uint i=-1,j=0;
  while (disk.inodes[j].first_byte!=0 && j<INODE_TABLE_SIZE) j++;
  if (j<INODE_TABLE_SIZE) i=j;
  return i;
}

int init_inode(char *filename, int pos, int size) {
  int i=get_unused_inode(),j=0;
  if (i!=-1){
    while (filename[j]!='\0') {
      disk.inodes[i].filename[j]=filename[j];
      j++;
    }
    disk.inodes[i].filename[j]=filename[j];
    disk.inodes[i].size=size;
    disk.inodes[i].first_byte=pos;
    update_first_free_byte(compute_nblock(disk.super_block.first_free_byte+size)*BLOCK_SIZE);
    disk.super_block.number_of_files++;
    return i;
  }
  return -1;
}

void print_inode_table(){
  int i;
  printf("\n* INODES TABLE *\n");
  printf("---------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
  printf("| %s | %-32s | %s | %s | %s | %s | %-26s | %-26s | %s | %s |\n","INODE NUM","filename","size","uid","uright","oright","ctimestamp","mtimestamp","nblock","first_byte");
  for (i=0;i<INODE_TABLE_SIZE;i++){
    printf("| %-9d | %-32s | %-4u | %-3u | %-6u | %-6u | %-26s | %-26s | %-6u | %-10u |\n",
    i,disk.inodes[i].filename,disk.inodes[i].size,disk.inodes[i].uid,disk.inodes[i].uright,disk.inodes[i].oright,disk.inodes[i].ctimestamp,disk.inodes[i].mtimestamp,disk.inodes[i].nblock,disk.inodes[i].first_byte);
  }
}

void print_superblock() {
  printf("\n* SUPERBLOCK *\nnumber of files :\t%u\nnumber_of_users :\t%u\nnb_blocks_used :\t%u\nfirst_free_byte :\t%u\n",
  disk.super_block.number_of_files,disk.super_block.number_of_users,disk.super_block.nb_blocks_used,disk.super_block.first_free_byte);
}
