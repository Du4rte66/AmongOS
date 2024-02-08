/**
 * @file couche_1.c
 * @brief contient code de la couche 1
 * @author romeo,duarte,chedli,elyes
 * @date 13/03/2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Utils/utils.h"

/**
 * @brief initialise la variable gloable representant le disque
 */
void init_disk_os(const char *dirname){
  char filename[50]="";
  strcat(filename,dirname);
  strcat(filename,"/d0");
  disk.storage=fopen(filename, "rb+");
}

/**
 * @return n/BLOCK_SIZE(+1 if %!=0);
 */
size_t compute_nblock(size_t n){
  size_t a=0;
  if (n%BLOCK_SIZE!=0) a++;
  return n/BLOCK_SIZE+a;
}

int read_block(void *buffer, long pos, size_t taille){
  fseek(disk.storage, pos, SEEK_SET);
  if (!fread(buffer, BLOCK_SIZE, compute_nblock(taille), disk.storage))
  {
    fprintf(stderr, "Erreur : Impossible de lire dans la destination donnée\n");
    return 1;
  }
  return 0;
}

int write_block(void *buffer, long pos, size_t taille){
  fseek(disk.storage, pos, SEEK_SET);
  if (!fwrite(buffer, BLOCK_SIZE, compute_nblock(taille), disk.storage))
  {
    fprintf(stderr, "Erreur : Impossible d'écrire dans la destination donnée\n");
    return 1;
  }
  return 0;
}

/**
 * @brief Eteint le systeme \
 * (ecrit une derniere fois la variable dans le fichier et ferme)
 */
void shutDown_disk_os(){
  /* FERMETURE FICHIER */
  fclose(disk.storage);
}
