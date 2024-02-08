/**
 * @file couche_3.h
 * @brief contient l'interface de la couche 3
 * @author romeo,duarte,chedli,elyes
 * @date 04/2022
 */

#ifndef _COUCHE3_
#define _COUCHE3_

/**
 * @brief lis la table des utilisateurs sur le disque */
void read_user_table();

/**
 * @brief ecrit la table des utilisateurs sur le disque */
void write_user_table();

/**
 * @brief crée le root
 * @return 0 si le root est créé, 1 sinon*/
int create_root();

/**
 * @brief ajoute un utilisateur
 * @return 0 si utilisateur créé, 1 sinon */
int add_user(char* login, char* password);

/**
 * @brief supprime un utilisateur
 * @return 0 si utilisateur est supprimé, 1 sinon */
int delete_user(int id_user);

/**
 * @brief affiche la table d'utilisateur sous forme :\n
 * iud : login */
void print_user_table();

/**
 * @brief modifie le nom de l'utilisateur \a user_id
 * @return 0 si utilisateur est modifié, 1 sinon */
int modify_login(int user_id,char* log);

/**
 * @brief modifie le mot de passe de l'utilisateur \a user_id
 * @return 0 si utilisateur est modifié, 1 sinon */
int modify_password(int user_id,char* password);

/**
 * @brief connecte la session à un utilisateur
 * @return Id de l'utilisateur ou -1 si aucun identifiant correspondant*/
int connection();

#endif
