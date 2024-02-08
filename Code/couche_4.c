/**
 * @file couche_4.c
 * @brief contient code de la couche 4
 * @author romeo,duarte,chedli,elyes
 * @date 03/2022
 */

#include <stdio.h>
#include <string.h>
#include "../Utils/utils.h"
#include "../Utils/timestamp.h"

#include "couche_1.h"
#include "couche_2.h"
#include "couche_3.h"
#include "couche_4.h"

/**
 * @brief cherche le nom d'un fichier dans la table des inode
 * @return -1 si pas trouvé, id de l'inode sinon
 */
int search_filename_in_table(char* filename) {
  int j=0;
  while (j<INODE_TABLE_SIZE) {
    if (disk.inodes[j].first_byte!=0) {
      if (strcmp(filename,disk.inodes[j].filename)==0)
        break;
    }
    j++;
  }
  if (j==INODE_TABLE_SIZE)
    return -1;
  return j;
}

int right_to_write(int inode_id) {
  if (!session.userid) //if root
    return 1;

  const inode_t* temp=&(disk.inodes[inode_id]);

  if (session.userid==temp->uid) //if owner
    return temp->uright==rW || temp->uright==RW;

  return temp->oright==rW || temp->oright==RW; //if other
}

int right_to_read(int inode_id) {
  if (!session.userid) //if root
    return 1;

  const inode_t* temp=&(disk.inodes[inode_id]);

  if (session.userid==temp->uid) //if owner
    return temp->uright==Rw || temp->uright==RW;

  return temp->oright==Rw || temp->oright==RW; //if other
}

int write_file(char* filename, file_t file){
  int i=search_filename_in_table(filename);
  inode_t* temp;
  if (i==-1) {
    if ((i=init_inode(filename,disk.super_block.first_free_byte,file.size))==-1) //Aucun inode disponible
      return 1;

    int vide=file.size;
    while (vide%BLOCK_SIZE) {
      file.data[vide]='\0';
      vide++;
    }
    if (vide<MAX_FILE_SIZE) file.data[vide]='\0';
    temp=&(disk.inodes[i]);
    temp->uid     = session.userid;
    temp->uright  = RW;
    temp->oright  = Rw;
    strcpy(temp->ctimestamp,timestamp());
    strcpy(temp->mtimestamp,temp->ctimestamp);
    temp->nblock  = compute_nblock(file.size);
    disk.super_block.nb_blocks_used+=temp->nblock;

    write_block(file.data,temp->first_byte,temp->size);
    return 0;
  }
  if (!right_to_write(i)) //Impossible de modifier le fichier, pas le droit
    return 2;

  temp=&(disk.inodes[i]);
  if (temp->size >= file.size) {
    temp->size    = file.size;
    disk.super_block.nb_blocks_used-=temp->nblock;
    temp->nblock  = compute_nblock(file.size);
    disk.super_block.nb_blocks_used+=temp->nblock;
    temp->first_byte = disk.super_block.first_free_byte;
    update_first_free_byte(compute_nblock(disk.super_block.first_free_byte+file.size)*BLOCK_SIZE);
    strcpy(temp->mtimestamp,timestamp());
  }
  else {
    delete_inode(i);
    i=init_inode(filename,disk.super_block.first_free_byte,file.size);
    int vide=file.size;
    while (vide%BLOCK_SIZE) {
      file.data[vide]='\0';
      vide++;
    }
    if (vide<MAX_FILE_SIZE) file.data[vide]='\0';
    temp=&(disk.inodes[i]);
    temp->uid     = session.userid;
    temp->uright  = RW;
    temp->oright  = Rw;
    strcpy(temp->ctimestamp,timestamp());
    strcpy(temp->mtimestamp,temp->ctimestamp);
    temp->nblock  = compute_nblock(file.size);
    disk.super_block.nb_blocks_used+=temp->nblock;
  }
  write_block(file.data,temp->first_byte,temp->size);
  return 0;
}

int read_file(char* filename, file_t* file){
  int i=search_filename_in_table(filename);
  const inode_t* temp;
  if (i==-1) //fichier introuvable
    return 1;

  else if (!right_to_read(search_filename_in_table(filename)))
    return 2; //droits de lecture

  temp=&(disk.inodes[i]);
  file->size=temp->size;
  if (read_block(file->data,temp->first_byte,file->size)==1) //lecture des données impossible
    return 3;

  return 0;
}

int delete_file(char* filename) {
  int i=search_filename_in_table(filename);
  if (i==-1) //fichier introuvable
    return 1;

  else if (!right_to_write(search_filename_in_table(filename)))
    return 2; //droits d'écriture

  delete_inode(i);
  return 0;
}

int load_file_from_host(char* filename) {
  FILE* f=fopen(filename,"rt");
  file_t file;
  int i=0;
  char c;
  while (i<MAX_FILE_SIZE && (c=fgetc(f))!=EOF) {
    file.data[i]=c;
    i++;
  }
  file.size=i;
  if (write_file(filename,file)==1) //Erreur ecriture du fichier
    return 1;

  printf("Fichier copie sur AmongOS\n");
  fclose(f);
  return 0;
}

int store_file_to_host(char* filename) {
  FILE* f=fopen(filename,"w");
  file_t file;
  if (read_file(filename,&file)==1) //Erreur lecture du fichier
    return 1;

  fwrite(file.data,1,file.size,f);
  printf("Fichier copie sur l'hote.\n");

  fclose(f);
  return 0;
}
