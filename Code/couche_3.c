/**
 * @file couche_3.c
 * @brief contient code de la couche 3
 * @author romeo,duarte,chedli,elyes
 * @date 03/2022
 */

#include <stdio.h>
#include <string.h>


#include "../Utils/utils.h"
#include "../Utils/Sha256/sha256.h"
#include "../Utils/Sha256/sha256_utils.h"
#include "couche_1.h"
#include "couche_3.h"

int check_uid() {
  int i=1;
  while (i<NB_USERS && disk.users_table[i].login[0]!='\0') i++;
  return i;
}

void read_user_table() {
  read_block(&disk.users_table,USERS_TABLE_START,sizeof(users_table_t));
}

void write_user_table() {
  write_block(&disk.users_table,USERS_TABLE_START,sizeof(users_table_t));
}

int create_root() {
  if (disk.users_table[0].login[0]!='\0') {
    fprintf(stderr,"Root deja cree\n");
    return 1;
  }
  char password[SHA256_BLOCK_SIZE*2 + 1],c;
  printf("Creatig root\n");
  strcpy(disk.users_table[0].login,"root");
  printf("ENTER PASSWORD : ");
  int i=0;
  while (i<65 && (c=getchar())!='\n') password[i++]=c;
  if (i>=65) i=64;
  password[i]='\0';
  int bufferSize = SHA256_BLOCK_SIZE;
	char hashRes[bufferSize*2 + 1];
	char * item = malloc(strlen(password)*sizeof(char));
	strcpy(item,password);
	sha256ofString((BYTE *)item,hashRes);

  strcpy(disk.users_table[0].passwd,hashRes);
  disk.super_block.number_of_users++;
  return 0;
}

int add_user(char* login, char* password){
  if(disk.super_block.number_of_users == NB_USERS){
    fprintf(stderr,"Erreur, nombre d'utilisateurs maximal atteint.\n");
    return 1;
  }
  int uid=check_uid();
  strcpy(disk.users_table[uid].login,login);

  int bufferSize = SHA256_BLOCK_SIZE;
	char hashRes[bufferSize*2 + 1];
	char * item = malloc(strlen(password)*sizeof(char));
	strcpy(item,password);
	sha256ofString((BYTE *)item,hashRes);

  strcpy(disk.users_table[uid].passwd,hashRes);
  disk.super_block.number_of_users++;
  return 0;
}


int delete_user(int id_user){
  if (disk.super_block.number_of_users <= 1) {
    fprintf(stderr,"AUCUN UTILISATEUR A SUPPRIMER\n");
    return 1;
  }
  if(id_user > NB_USERS-1 || id_user < 1 || disk.users_table[id_user].login[0]=='\0') {
    fprintf(stderr,"ID INVALIDE\n");
    return 2;
  }
  disk.users_table[id_user].login[0]='\0';
  disk.super_block.number_of_users--;
  return 0;
}

void print_user_table() {
  printf("\n* USER_TABLE * \n");
  printf("-------------------------------------------------------------------------------------------------------------\n");
  printf("| %-3s | %-32s | %-64s |\n","UID","LOGIN","PASSWORD");
  for(int i = 0; i<NB_USERS; i++){
    printf("| %-3d | %-32s | %-64s |\n",i,disk.users_table[i].login,disk.users_table[i].passwd);
  }

}

int modify_login(int user_id,char* log){
  if(disk.super_block.number_of_users < user_id || user_id == 0) {
    perror("Erreur id\n");
    return 1;
  }
  strcpy(disk.users_table[user_id].login,log);
  return 0;
}

int modify_password(int user_id,char* password){
  if(disk.super_block.number_of_users < user_id || user_id == 0) {
    perror("Erreur id\n");
    return 1;
  }
  int bufferSize = SHA256_BLOCK_SIZE;
	char hashRes[bufferSize*2 + 1]; // contiendra le hash en hexadécimal
	char * item = malloc(strlen(password)*sizeof(char)); // contiendra la chaîne à hasher
	strcpy(item,password); // c'est elle
	sha256ofString((BYTE *)item,hashRes); // hashRes contient maintenant le hash de l'item

  strcpy(disk.users_table[disk.super_block.number_of_users].passwd,hashRes);
  return 0;
}

int connection(){
  char login[FILENAME_MAX_SIZE];
  char password[65];

  printf("\n--//CONNECTION\\\\--\n");
  printf("LOGIN : ");
  int i=0;
  char c;
  while (i<FILENAME_MAX_SIZE && (c=getchar())!='\n') login[i++]=c;
  if (i>=FILENAME_MAX_SIZE) i=FILENAME_MAX_SIZE-1;
  login[i]='\0';
  // scanf("%s",login); UNSAFE
  printf("PASSWORD : ");
  i=0;
  while (i<65 && (c=getchar())!='\n') password[i++]=c;
  if (i>=65) i=64;
  password[i]='\0';
  // scanf("%s",password); UNSAFE

  //Hashage du mot de passe pour le retrouver
  int bufferSize = SHA256_BLOCK_SIZE;
  char hashRes[bufferSize*2 + 1];
  char * item = malloc(strlen(password)*sizeof(char));
  strcpy(item,password);
  sha256ofString((BYTE *)item,hashRes);

  //On cherche les infos pour identifier le user
  for(int i=0; i<NB_USERS; i++){
    if(strcmp(disk.users_table[i].passwd,hashRes) == 0
        && strcmp(disk.users_table[i].login,login) == 0){
          return i;
        }
  }

  return -1;
}
