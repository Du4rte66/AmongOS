#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "Utils/utils.h"
int main(){
  printf("INODE START = %d\nINODE SIZE = %d\nCHAMP = %d\n",INODES_START,INODE_SIZE*BLOCK_SIZE,FILENAME_MAX_SIZE+4+4+4+4+TIMESTAMP_SIZE+TIMESTAMP_SIZE+4);
  return 0;
}
