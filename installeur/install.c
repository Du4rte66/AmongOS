#include <stdio.h>

#include "../Utils/utils.h"
#include "../Code/couche_1.h"
#include "../Code/couche_2.h"
#include "../Code/couche_3.h"
#include "../Code/couche_4.h"

int main() {
  printf("INSTALLATION SYSTEME\n");
  init_disk_os("dirdisk");
  read_super_block();
  read_inodes_table();
  read_user_table();
  update_first_free_byte(FIRST_FREE_BYTE_START); //A EXECUTER SEULEMENT AU PREMIER DEMARAGE
  create_root();
  write_inodes_table();
  write_super_block();
  write_user_table();
  shutDown_disk_os(disk);
  printf("Installation terminée\n");
  return 0;
}
