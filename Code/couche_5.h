/**
 * @file couche_5.h
 * @brief contient l'interface de la couche 5
 * @author romeo,duarte,chedli,elyes
 * @date 04/2022
 */

#ifndef _COUCHE5_
#define _COUCHE5_

#include "../Utils/utils.h"

/**
 * @brief lit une commande en lisant stdin
 * @return pointer vers une commande de type cmd_t
 */
cmd_t* read_command();

/**
 * @brief libere une commande de type cmd_t
 */
void free_command(cmd_t* command);

/**
 * @brief execute la commande
 * @return 0 si quit est executée > sinon
 */
int execute_command(cmd_t command);

/**
 * @brief affiche la liste des fichiers du systeme
 */
void ls(cmd_t command);

/**
 * @brief affiche le contenu d'un fichier
 */
void cat(cmd_t command);

/**
 * @brief supprime un fichier
 */
void rm(cmd_t command);

/**
 * @brief créer un fichier
 */
void cr(cmd_t command);

/**
 * @brief modifie un fichier
 */
void edit(cmd_t command);

/**
 * @brief copie un fichier
 */
void cp(cmd_t command);

/**
 * @brief renomme un fichier
 */
void rename_file(cmd_t command);

/**
 * @brief charge un fichier depuis le systeme hote dans le systeme AmongOS
 */
void load(cmd_t command);

/**
 * @brief charge un fichier depuis le systeme AmongOS dans le systeme hote
 */
void store(cmd_t command);

/**
 * @brief change l'uid d'un inode
 */
void chown(cmd_t command);

/**
 * @brief change le mode d'un fichier
 */
void chmod(cmd_t command);

/**
 * @brief liste les utilisateurs du systeme
 */
void listuser(cmd_t command);

/**
 * @brief quite et sauvegarde
 */
int quit(cmd_t command);

/**
 * @brief ajoute un utilisateur
 */
void adduser(cmd_t command);

/**
 * @brief supprime un utilisateur
 */
void rmuser(cmd_t command);

/**
 * @brief affiche le manuel des commandes
 */
void help();

#endif
