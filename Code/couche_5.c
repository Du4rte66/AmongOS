/**
 * @file couche_5.c
 * @brief contient code de la couche 5
 * @author romeo,duarte,chedli,elyes
 * @date 05/2022
 */

#include <stdio.h>
#include <string.h>

#include "couche_1.h"
#include "couche_2.h"
#include "couche_3.h"
#include "couche_4.h"
#include "couche_5.h"
#include "../Utils/utils.h"
#define MAX_CMD 10 //nombre max d'arguments en commande
#define MAX_AFF 1024

cmd_t* read_command() {
  //lecture de stdin
  char c;
  cmd_t *out=malloc(sizeof(cmd_t));
  int i=0,len=0,bool_last_is_space=0,bool_last_is_letter=0;
  out->tabArgs=malloc(MAX_CMD*sizeof(char*));
  out->tabArgs[0]=malloc(50*sizeof(char));
  out->nbArgs=1;
  while((c=getchar())!='\n') {
    switch (c) {
      case ' ':
        i++;
        bool_last_is_space=1;
        if (bool_last_is_letter) {
          out->tabArgs[i-1][len]='\0';
          out->tabArgs[i-1]=realloc(out->tabArgs[i-1],(len+1)*sizeof(char));
        }
        len=0;
        bool_last_is_letter=0;
        break;
      default:
        if (bool_last_is_space) {
          out->nbArgs++;
          out->tabArgs[i]=malloc(50*sizeof(char));
          bool_last_is_space=0;
        }
        out->tabArgs[i][len]=c;
        len++;
        bool_last_is_letter=1;
    }
  }
  if (bool_last_is_letter) {
    out->tabArgs[i][len]='\0';
    out->tabArgs[i]=realloc(out->tabArgs[i],(len+1)*sizeof(char));
    i++;
  }
  return out;
}

void free_command(cmd_t* command) {
  for (int i=0; i<command->nbArgs;i++)
    free(command->tabArgs[i]);
  free(command);
}

int command_to_code(cmd_t command) {
  if (strcmp(command.tabArgs[0],"quit")==0)
    return 0;
  if (strcmp(command.tabArgs[0],"ls")==0)
    return 1;
  if (strcmp(command.tabArgs[0],"cat")==0)
    return 2;
  if (strcmp(command.tabArgs[0],"rm")==0)
    return 3;
  if (strcmp(command.tabArgs[0],"cr")==0)
    return 4;
  if (strcmp(command.tabArgs[0],"edit")==0)
    return 5;
  if (strcmp(command.tabArgs[0],"load")==0)
    return 6;
  if (strcmp(command.tabArgs[0],"store")==0)
    return 7;
  if (strcmp(command.tabArgs[0],"chown")==0)
    return 8;
  if (strcmp(command.tabArgs[0],"chmod")==0)
    return 9;
  if (strcmp(command.tabArgs[0],"listuser")==0)
    return 10;
  if (strcmp(command.tabArgs[0],"adduser")==0)
    return 11;
  if (strcmp(command.tabArgs[0],"rmuser")==0)
    return 12;
  if (strcmp(command.tabArgs[0],"cp")==0)
    return 13;
  if (strcmp(command.tabArgs[0],"rename")==0)
    return 14;
  if (strcmp(command.tabArgs[0],"help")==0)
    return 15;
  return -1;

}

int execute_command(cmd_t command) {
  int code=command_to_code(command);
  switch(code) {
    case 0: quit(command); break;
    case 1: ls(command); break;
    case 2: cat(command); break;
    case 3: rm(command); break;
    case 4: cr(command); break;
    case 5: edit(command); break;
    case 6: load(command); break;
    case 7: store(command); break;
    case 8: chown(command); break;
    case 9: chmod(command); break;
    case 10: listuser(command); break;
    case 11: adduser(command); break;
    case 12: rmuser(command); break;
    case 13: cp(command); break;
    case 14: rename_file(command); break;
    case 15: help(); break;
    default: fprintf(stderr,"Invalid command\n");
  }
  return code;
}

void ls(cmd_t command){
  if ((command.nbArgs == 2) && (command.tabArgs[1][0] == '-')) { //commande -parametre
    /*   Cas "ls -l" (long)   */
    if ((command.tabArgs[1][1] == 'l') && (command.tabArgs[1][2] == '\0')) {
      char affichage[MAX_AFF] = "";
      char buffer[16];
      uint total = 0;

      for (int i = 0; strlen(disk.inodes[i].filename) && i < INODE_TABLE_SIZE; i ++) {
        /*   Affichage des droits   */
        if (disk.inodes[i].uright >= 2) {strcat(affichage, "r");}//user read
        else {strcat(affichage, "-");}

        if (disk.inodes[i].uright % 2 == 1) {strcat(affichage, "w ");}//user write
        else {strcat(affichage, "- ");}

        if (disk.inodes[i].oright >= 2) {strcat(affichage, "r");}//other read
        else {strcat(affichage, "-");}

        if (disk.inodes[i].oright % 2 == 1) {strcat(affichage, "w ");}//other write
        else {strcat(affichage, "- ");}

        /*   Nom de l'owner   */
        strcat(affichage, disk.users_table[disk.inodes[i].uid].login);

        /*   Taille du fichier   */
        sprintf(buffer, " %d ", disk.inodes[i].size);
        strcat(affichage, buffer);

        /*   Dernière modification du fichier   */
        strcat(affichage, disk.inodes[i].mtimestamp);
        strcat(affichage, " ");

        /*   Nom du fichier   */
        strcat(affichage, GREEN);
        strcat(affichage, disk.inodes[i].filename);
        strcat(affichage, RESET);
        strcat(affichage, "\n");

        /*   Calcul du poids total des fichier   */
        total += disk.inodes[i].size;
      }
      /*   Affichage de la commande "ls -l"   */
      printf("Total : %d Octet(s)\n", total);
      printf("%s", affichage);
    }
    /*   Cas "ls -c" (court)   */
    else if ((command.tabArgs[1][1] == 'c') && (command.tabArgs[1][2] == '\0')) {
      for (int i = 0; i < INODE_TABLE_SIZE; i ++) {
        if (disk.inodes[i].filename[0] != '\0')
          printf("%s%s%s : %d o\n", GREEN, disk.inodes[i].filename, RESET, disk.inodes[i].size);
      }
    }
    else {fprintf(stderr, "Usage : ls [-l] [-c] (les deux paramètres ne peuvent pas être utilisés ensemble)\n");} //usage
  }
  /*   Cas "ls" (sans parametre)   */
  else if (command.nbArgs == 1) {
    printf("%s", GREEN);
    for (int i = 0; i < INODE_TABLE_SIZE; i ++) {
      if (disk.inodes[i].filename[0] != '\0')
        printf("%s  ", disk.inodes[i].filename);
    }
    printf("%s\n", RESET);
  }
  /*   Erreur   */
  else {fprintf(stderr, "Usage : ls [-l] [-c]\n");}
}

void cat(cmd_t command) {
  if ((command.nbArgs >= 2)) { //commande <designation>
    file_t f;
    for (int i = 1; i < command.nbArgs; i++) {
      int code = read_file(command.tabArgs[i], &f);
      if (!code) {
        for(int j = 0; j < f.size; j++)
          printf("%c", f.data[j]);
        printf("\n");
      }

      else {
        fprintf(stderr, "[%sERREUR%s] %s : ",RED, RESET, command.tabArgs[i]);
        if (code == 1)
          fprintf(stderr, "Fichier introuvable\n");
        else if (code == 2)
          fprintf(stderr, "Vous ne possédez pas les droits de lecture de ce fichier\n");
        else if (code == 3)
          fprintf(stderr, "Lecture des données impossible\n");
      }
    }
  }
  else {fprintf(stderr, "Usage : cat <designation>\n");}
}

void rm(cmd_t command) {
  if ((command.nbArgs >= 2)) { //commande <designation>
    for (int i = 1; i < command.nbArgs; i++) {
      int code = delete_file(command.tabArgs[i]);
      if (!code)
          printf("%s%s%s : Fichier supprimé\n", GREEN, command.tabArgs[i], RESET);
      else {
        fprintf(stderr,"[%sERREUR%s] %s : ",RED, RESET, command.tabArgs[i]);
        if (code == 1)
          fprintf(stderr,"Fichier introuvable\n");
        else if (code == 2)
          fprintf(stderr, "Vous ne possédez pas les droits d'écriture de ce fichier\n");
      }
    }
  }
  else {fprintf(stderr, "Usage : rm <designation>\n");} //usage
}

void cr(cmd_t command) {
  if (command.nbArgs == 2){ //commande -parametre
    /*   <designation> trop longue   */
    if (strlen(command.tabArgs[1]) > FILENAME_MAX_SIZE - 1) //31
      fprintf(stderr, "[%sERREUR%s] %s : Le nom du fichier dépasse 31 caractères\n", RED, RESET, command.tabArgs[1]);

    /*   erreur si le fichier existe deja   */
    else if (search_filename_in_table(command.tabArgs[1]) != -1)
      fprintf(stderr, "[%sERREUR%s] %s : Le fichier existe deja.\n", RED, RESET, command.tabArgs[1]);

    /*   Creation du nouveau fichier   */
    else {
      file_t f;
      printf("Edition de %s%s%s : (Echap entrée pour valider)\n", GREEN, command.tabArgs[1], RESET);
      /*   Saisie des données   */
      for (f.size = 0; (f.data[f.size] = getchar()) != 27; ++ f.size);
      getchar();
      f.data[f.size ++] = '\0';

      /*   Enregistrement des données dans le fichier   */
      int code = write_file(command.tabArgs[1], f);

      if (!code)
        disk.inodes[search_filename_in_table(command.tabArgs[1])].uid = session.userid;

      else {
        fprintf(stderr, "[%sERREUR%s] %s : ", RED, RESET, command.tabArgs[1]);
        if (code == 1)
          fprintf(stderr, "Aucun inode disponible\n");
        else if (code == 2)
          fprintf(stderr, "Ecriture du fichier impossible\n");
      }
    }
  }
  else {fprintf(stderr, "Usage : cr <designation>\n");}
}


void edit(cmd_t command) {
  if (command.nbArgs == 2 || command.nbArgs == 3) { //commande [-parametre] <designation>
    int i = 1;

    if ((command.nbArgs == 3) && (!strcmp(command.tabArgs[1],"-s"))) //edit -s (show)
      i = 2;

    /*  Si le fichier existe */
    if (search_filename_in_table(command.tabArgs[i]) != -1) {
      file_t f;
      int code;
      if (i == 2) { //cas edit -s    on affiche le contenu d'abord
        code = read_file(command.tabArgs[i], &f);
        if (!code) {
          /*   Affichage du contenu du fichier   */
          printf("%s%s%s : %d Octets\n", GREEN, command.tabArgs[i], RESET, f.size);
          for(int j = 0; j < f.size; j++)
            printf("%c", f.data[j]);
          printf("\n\n");
        }
        else { //erreur
          fprintf(stderr, "[%sERREUR%s] %s : ", RED, RESET, command.tabArgs[i]);
          if (code == 1)
            fprintf(stderr, "Fichier introuvable\n");
          else if (code == 2)
            fprintf(stderr, "Vous ne possédez pas les droits de lecture de ce fichier\n");
          else if (code == 3)
            fprintf(stderr, "Lecture des données impossible\n");
        }
      }

      if (right_to_write(search_filename_in_table(command.tabArgs[i]))) {
        printf("Edition de %s%s%s : (Echap entrée pour valider)\n", GREEN, command.tabArgs[i], RESET);

        /*   Saisie des nouvelles données   */
        for (f.size = 0; (f.data[f.size] = getchar()) != 27; ++ f.size);
        getchar();
        f.data[f.size ++] = '\0';

        code = write_file(command.tabArgs[i], f);

        if (code) {
          fprintf(stderr, "[%sERREUR%s] %s : ", RED, RESET, command.tabArgs[i]);
          if (code == 1)
            fprintf(stderr, "Aucun inode disponible\n");
          else if (code == 2)
            fprintf(stderr, "Vous ne possédez pas les droits d'écriture de ce fichier\n");
        }
      }
      else //erreur droits
        fprintf(stderr, "[%sERREUR%s] %s : Vous ne possédez pas les droits d'écriture de ce fichier\n", RED, RESET, command.tabArgs[i]);
    }
    else //fichier introuvable
      fprintf(stderr, "[%sERREUR%s] %s : Fichier introuvable\n", RED, RESET, command.tabArgs[i]);
  }
  else {fprintf(stderr, "Usage : edit [-s] <nom_de_fichier>\n");} //usage
}

void cp(cmd_t command){
  if (command.nbArgs == 3) {
    if (search_filename_in_table(command.tabArgs[1]) != -1) {
        if (right_to_read(search_filename_in_table(command.tabArgs[1]))) { //droit de lecture
          if (((search_filename_in_table(command.tabArgs[2]) != -1) && (right_to_write(search_filename_in_table(command.tabArgs[2])))) ||
               (search_filename_in_table(command.tabArgs[2]) == -1)){//si (exite et drois) ou (n'existe pas)
            file_t f;
            int code = read_file(command.tabArgs[1], &f);
            if (!code) {
              code = write_file(command.tabArgs[2], f);

              if (code) {
                fprintf(stderr, "[%sERREUR%s] %s : ", RED, RESET, command.tabArgs[2]);
                if (code == 1)
                  fprintf(stderr, "Aucun inode disponible\n");
                else if (code == 2)
                  fprintf(stderr, "Vous ne possédez pas les droits d'écriture de ce fichier\n");
              }
            }

            else {
              fprintf(stderr, "[%sERREUR%s] %s : ",RED, RESET, command.tabArgs[1]);
              if (code == 1)
                fprintf(stderr, "Fichier introuvable\n");
              else if (code == 2)
                fprintf(stderr, "Vous ne possédez pas les droits de lecture de ce fichier\n");
              else if (code == 3)
                fprintf(stderr, "Lecture des données impossible\n");
            }
          }
          else {fprintf(stderr, "[%sERREUR%s] %s :  Vous ne possédez pas les droits d'écriture de ce fichier.\n", RED, RESET, command.tabArgs[1]);}
        }
        else {fprintf(stderr, "[%sERREUR%s] %s :  Vous ne possédez pas les droits de lecture de ce fichier.\n", RED, RESET, command.tabArgs[1]);}
    }
    else {fprintf(stderr, "[%sERREUR%s] %s : Le fichier n'existe pas.\n", RED, RESET, command.tabArgs[1]);}
  }
  else {fprintf(stderr, "Usage : cp <nom_de_fichier> <fichier_destination>\n");} //usage
}

void rename_file(cmd_t command){
  if (command.nbArgs == 3) {
    if (search_filename_in_table(command.tabArgs[1]) != -1) { //fichier existe
      if (right_to_write(search_filename_in_table(command.tabArgs[1]))) {
        /*   erreur si le fichier existe deja   */
        if (search_filename_in_table(command.tabArgs[2]) != -1)
          fprintf(stderr, "[%sERREUR%s] %s : Le fichier existe deja.\n", RED, RESET, command.tabArgs[2]);
        else {
          /*   <designation> trop longue   */
          if (strlen(command.tabArgs[1]) > FILENAME_MAX_SIZE - 1) //31
            fprintf(stderr, "[%sERREUR%s] %s : Le nom du fichier dépasse 31 caractères\n", RED, RESET, command.tabArgs[2]);
          else {
            strcpy(disk.inodes[search_filename_in_table(command.tabArgs[1])].filename, command.tabArgs[2]);
            printf("Le fichier %s%s%s à été renommé en %s%s%s\n", GREEN, command.tabArgs[1], RESET, GREEN, command.tabArgs[2], RESET);
          }
        }
      }
      else {fprintf(stderr, "[%sERREUR%s] %s :  Vous ne possédez pas les droits d'écriture de ce fichier.\n", RED, RESET, command.tabArgs[1]);}
    }
    else {fprintf(stderr, "[%sERREUR%s] %s : Le fichier n'existe pas.\n", RED, RESET, command.tabArgs[1]);}
  }
  else {fprintf(stderr, "Usage : rename <nom_de_fichier> <nouveau nom>\n");} //usage
}

void load(cmd_t command) {
  if (command.nbArgs != 2) {
    fprintf(stderr,"Usage : load <designation>\n");
  }
  else{
    if (load_file_from_host(command.tabArgs[1])==1)
      fprintf(stderr, "Impossible de charger le fichier, introuvable ou inaccessible\n");
  }
}

void store(cmd_t command) {
  if (command.nbArgs == 2){
    if (store_file_to_host(command.tabArgs[1]))
      fprintf(stderr, "Impossible de charger le fichier, inaccessible\n");
  }
  else {fprintf(stderr,"Usage : store <designation>\n");}
}

int search_login(char *name){
  int id = -1;
  for (int i = 0; i < NB_USERS; i++) {
    if(!strcmp(name, disk.users_table[i].login))
      id =  i;
  }
  return id;
}

void chown(cmd_t command) {
  if(command.nbArgs == 3) {
    int proprietaire = search_filename_in_table(command.tabArgs[1]); //recherche de l'id du propriétaire du fichier
    int user = search_login(command.tabArgs[2]); //recherche de l'id de l'user donné
    if(proprietaire != -1){ //si le fichier existe
      if(disk.inodes[proprietaire].uid != user){ //si les deux users sont differents
        if(user != -1){ //si l'user existe
          if (right_to_write(proprietaire)) { //verif droits
            disk.inodes[proprietaire].uid=user;
          }
          else {fprintf(stderr, "[%sERREUR%s] %s :  Vous ne possédez pas les droits d'écriture de ce fichier.\n", RED, RESET, command.tabArgs[1]);}
        }
        else {fprintf(stderr, "[%sERREUR%s] %s : Cet utilisateur n'existe pas.\n", RED, RESET, command.tabArgs[2]);}
      }
      else {fprintf(stderr, "[%sERREUR%s] %s : est déja le proprietaire du fichier.\n", RED, RESET, command.tabArgs[2]);}
    }
    else {fprintf(stderr, "[%sERREUR%s] %s : Le fichier n'existe pas.\n", RED, RESET, command.tabArgs[1]);}
  }
  else {printf("Usage : chown <designation> <nom_user>\n");}
}

void chmod(cmd_t command) {
  if(command.nbArgs >= 3) {
    if (!strcmp(command.tabArgs[1],"-o") || !strcmp(command.tabArgs[1],"-ou") || !strcmp(command.tabArgs[1],"-uo")) { //chmod -o (other) rw nom_de_fichier
      for (int i = 3; i < command.nbArgs; i++) { //chaque argument
        int j = search_filename_in_table(command.tabArgs[i]);
        if (j != -1){ //si le fichier existe
          if(right_to_write(search_filename_in_table(command.tabArgs[i]))){ //droits
            if     (!strcmp(command.tabArgs[2],"rw")) {disk.inodes[j].oright = rw;}
            else if(!strcmp(command.tabArgs[2],"rW")) {disk.inodes[j].oright = rW;}
            else if(!strcmp(command.tabArgs[2],"Rw")) {disk.inodes[j].oright = Rw;}
            else if(!strcmp(command.tabArgs[2],"RW")) {disk.inodes[j].oright = RW;}
            else {fprintf(stderr, "Usage : chmod [-u] [-o] <droits(rw/Rw/rW/RW)> <nom_de_fichier>\n");} //usage
          }
          else {fprintf(stderr, "[%sERREUR%s] %s :  Vous ne possédez pas les droits d'écriture de ce fichier.\n", RED, RESET, command.tabArgs[i]);}
        }
        else {fprintf(stderr, "[%sERREUR%s] %s : Le fichier n'existe pas.\n", RED, RESET, command.tabArgs[i]);}
      }
    }

    if (strcmp(command.tabArgs[1],"-o")){ //cas chmod rw nom_de_fichier
      int arg = 2;
      if (!strcmp(command.tabArgs[1],"-u") || !strcmp(command.tabArgs[1],"-ou") || !strcmp(command.tabArgs[1],"-uo")) { //chmod -u (user) rw nom_de_fichier
        arg = 3;
      }
      for (int i = arg; i < command.nbArgs; i++) { //chaque argument
        int j = search_filename_in_table(command.tabArgs[i]);
        if (j != -1){ //si le fichier existe
          if(right_to_write(search_filename_in_table(command.tabArgs[i]))){ //droits
            if     (!strcmp(command.tabArgs[arg-1],"rw")) {disk.inodes[j].uright = rw;}
            else if(!strcmp(command.tabArgs[arg-1],"rW")) {disk.inodes[j].uright = rW;}
            else if(!strcmp(command.tabArgs[arg-1],"Rw")) {disk.inodes[j].uright = Rw;}
            else if(!strcmp(command.tabArgs[arg-1],"RW")) {disk.inodes[j].uright = RW;}
            else {fprintf(stderr, "Usage : chmod [-u] [-o] <droits(rw/Rw/rW/RW)> <nom_de_fichier>\n");} //usage
          }
          else {fprintf(stderr, "[%sERREUR%s] %s :  Vous ne possédez pas les droits d'écriture de ce fichier.\n", RED, RESET, command.tabArgs[i]);}
        }
        else {fprintf(stderr, "[%sERREUR%s] %s : Le fichier n'existe pas.\n", RED, RESET, command.tabArgs[i]);}
      }
    }
  }
  else {fprintf(stderr, "Usage : chmod [-u] [-o] <droits(rw/Rw/rW/RW)> <nom_de_fichier>\n");} //usage
}

void listuser(cmd_t command){
  if (command.nbArgs == 1) {
    printf("Nombre d'utilisateurs : %d\n" , disk.super_block.number_of_users );
    for(uint i = 0 ; i < NB_USERS ; i++){
      if(disk.users_table[i].login[0] != '\0')
        printf("%s\n", disk.users_table[i].login);
    }
  }
  else {fprintf(stderr, "Usage : listuser\n");} //usage
}

int quit(cmd_t command) {
  if (command.nbArgs != 1) {
    fprintf(stderr,"Usage : quit\n");
    return 1;
  }
  printf("Sauvegarde des données\n");
  write_inodes_table();
  write_super_block();
  write_user_table();
  shutDown_disk_os(disk);

  return 0;
}

void adduser(cmd_t command){
  if (!session.userid) {
    if (command.nbArgs == 1) {
      char login[FILENAME_MAX_SIZE], pass[SHA256_BLOCK_SIZE * 2 + 1], buffer;
      printf("Login : ");
      int i = 0;
      while (i < FILENAME_MAX_SIZE && (buffer = getchar()) != '\n') login[i++] = buffer;
      if (i>=FILENAME_MAX_SIZE) i=64;
      login[i]='\0';
      if (search_login(login) != -1) {
        fprintf(stderr, "[%sERREUR%s] %s : Cet utilisateur existe déjà.\n", RED, RESET, login);
        adduser(command);
      }
      else {
        printf("Password : ");
        i = 0;
        while ((i < SHA256_BLOCK_SIZE * 2 + 1) && (buffer = getchar()) != '\n') pass[i++] = buffer;
        if (i>=65) i=64;
        pass[i]='\0';
        add_user(login,pass);
      }
    }
    else {fprintf(stderr,"Usage : adduser\n");}
  }
  else {fprintf(stderr, "[%sERREUR%s] adduser : fonction réservée au root.\n", RED, RESET);}
}

void rmuser(cmd_t command){
  if (!session.userid) { //si root
    if (command.nbArgs == 2) { //commande <utilisateur>
      if (disk.super_block.number_of_users > 1) { //nb ustilisateurs > 1
        int user = search_login(command.tabArgs[1]); //recherche de l'id de l'user donné
        if (user != -1) { //si l'utilisateur existe
          if (user != 0) { //si l'utilisateur n'est pas root
            delete_user(user);
          }
          else {fprintf(stderr, "[%sERREUR%s] %s : root n'est pas suppressible.\n", RED, RESET, command.tabArgs[1]);}
        }
        else {fprintf(stderr, "[%sERREUR%s] %s : Cet utilisateur n'existe pas.\n", RED, RESET, command.tabArgs[1]);}
      }
      else {fprintf(stderr, "[%sERREUR%s] %s : Il n'y a pas assez d'utilisateurs.\n", RED, RESET, command.tabArgs[1]);}
    }
    else {fprintf(stderr,"Usage : rmuser <utilisateur>\n");}
  }
  else {fprintf(stderr, "[%sERREUR%s] rmsuer : fonction réservée au root.\n", RED, RESET);}
}

void help(){
  printf("%sls [-l] [-c] :%s liste les fichiers du répertoire courant.\n", BOLDWHITE, RESET); //ls
  printf("%scat <désignation> :%s affiche le contenu du fichier désigné.\n", BOLDWHITE, RESET); //cat
  printf("%srm <désignation> :%s supprime le fichier désigné.\n", BOLDWHITE, RESET); //rm
  printf("%scr <désignation> :%s cree un nouveau fichier avec comme nom l'argument de la commande.\n", BOLDWHITE, RESET); //cr
  printf("%sedit [-s] <désignation> :%s édite le fichier désigné.\n", BOLDWHITE, RESET); //edit
  printf("%sload <désignation> :%s copie le contenu d un fichier du systeme hote sur le systeme avec le meme nom.\n", BOLDWHITE, RESET); //load
  printf("%scp <désignation> <destination> :%s copie le contenu du fichier désigné sur le fichier destination.\n", BOLDWHITE, RESET); //cp
  printf("%srename <désignation> <nouveau nom> :%s renomme le fichier désigné avec le nouveau nom.\n", BOLDWHITE, RESET); //rename
  printf("%sstore <désignation> :%s copie le contenu du fichier désigné sur l'hote avec le meme nom.\n", BOLDWHITE, RESET); //store
  printf("%schown <désignation> <utilisateur> :%s change le proprietaire du fichier désigné.\n", BOLDWHITE, RESET); //chown
  printf("%schmod [-u] [-o] <droit> <désignation> :%s change les droits du/des fichier(s) désigné(s).\n", BOLDWHITE, RESET); //chmod
  printf("%slistuser :%s affiche la liste des utilisateurs du systeme.\n", BOLDWHITE, RESET); //listuser
  printf("%sadduser :%s (root) ajoute un utilisateur.\n", BOLDWHITE, RESET); //adduser
  printf("%srmuser :%s (root) supprime un utilisateur.\n", BOLDWHITE, RESET); //rmuser
}
