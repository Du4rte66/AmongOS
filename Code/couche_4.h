/**
 * @file couche_4.h
 * @brief contient l'interface de la couche 4
 * @author romeo,duarte,chedli,elyes
 * @date 04/2022
 */

#ifndef _COUCHE4_
#define _COUCHE4_

#include "../Utils/utils.h"

int search_filename_in_table(char* filename);

int right_to_write(int inode_id);

int right_to_read(int inode_id);

/**
 * @brief ecrit un fichier, le crée si inexistant ou le modifie si inexistant
 * @param file structure contenant les données du fichiers
 * @return 0 si ok,1 si erreur
 */
int write_file(char* filename, file_t file);

/**
 * @brief lit un fichier
 * @param file structure contenant les données du fichiers lu
 * @return 0 si ok,1 si erreur
 */
int read_file(char* filename, file_t* file);

/**
 * @brief supprime l'inode un fichier
 * @return 0 si ok, >0 si erreur
 */
int delete_file(char* filename);

/**
 * @brief charge un fichier du systeme hote vers le systeme AmogUs
 * @return 0 si ok, 1 si erreur
 */
int load_file_from_host(char* filename);

/**
 * @brief charge un fichier du systeme AmogUs vers le systeme hote
 * @return 0 si ok, 1 si erreur
 */
int store_file_to_host(char* filename);

#endif
