#include <stdio.h>
#include <stdlib.h>

#include "../Utils/utils.h"
#include "couche_1.h"
#include "couche_2.h"
#include "couche_3.h"
#include "couche_4.h"
#include "couche_5.h"

/*Structure du main dans l'ordre:
 - init_disk_os("dirdisk"); OBLIGATOIRE
 - fonctions read_...() (initialise la variable disque)
 - A EXECUTER SEULEMENT AU PREMIER DEMARAGE :
   - update_first_free_byte(FIRST_FREE_BYTE_START);
   - create_root();
 - fonctions de création (add_user,init_inode,etc...)
 - fonctions write_...() (sauvegarde des données sur le disque, ici fichier)

 possibilité d'utiliser les fonctions print_...() n'importe ou APRES avoir initialisé
*/


int main(){

  /*--------------------
    Lecture des données
    ------------------*/
  init_disk_os("dirdisk");
  read_super_block();
  read_inodes_table();
  read_user_table();

  /*--------------------
  Installation systeme
  ------------------*/
  // update_first_free_byte(FIRST_FREE_BYTE_START); //A EXECUTER SEULEMENT AU PREMIER DEMARAGE
  // create_root();


  /*--------------------
    Connection user
    ------------------*/
  do {
    session.userid=connection();
  } while(session.userid == -1);
  int code;
  cmd_t* command;
  while (code!=0) {
    printf("%s%s%s:%s/%s$ ", BOLDGREEN, disk.users_table[session.userid].login, RESET, BLUE, RESET);
    command=read_command();
    code=execute_command(*command);
    free_command(command);
  }
  return 0;
}

// printf("\nTEST COUCHE 4\n");
// delete_inode(0);
// load_file_from_host("test.txt");
// print_inode_table();
// print_superblock();
// store_file_to_host("test.txt");


// printf("delete duarte : %d\n",delete_user(2));
// print_user_table();
// print_superblock();
// printf("ajout estel : %d\n", add_user("estel","estel"));
// print_user_table();
// print_superblock();


// printf("Creation Root\n");
// create_root();
// print_user_table();

// printf("TEST CONNECTION : %d\n",connection());
// printf("ajout romeo : %d\n", add_user("romeo","romeo"));
// printf("ajout duarte : %d\n", add_user("duarte portugais","duarte"));
// printf("ajout chedli : %d\n", add_user("chedli gay","chedli"));
// printf("ajout Elyes : %d\n", add_user("Elyes 3arbi","elyes"));
// printf("ajout test : %d\n",add_user("test","test"));

// print_superblock();
// print_user_table();
// printf("Update du premier octet libre\n");
// printf("Ecriture fichier 1 : 12 octets\n");
// init_inode("caca",disk.super_block.first_free_byte,12);
// printf("Ecriture fichier 2 : 25 octets\n");
// init_inode("pipi",disk.super_block.first_free_byte,25);

// printf("Affichage stat\n");
// print_superblock();
// print_inode_table();
