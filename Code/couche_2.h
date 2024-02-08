/**
 * @file couche_2.h
 * @brief contient l'interface de la couche 2
 * @author romeo,duarte,chedli,elyes
 * @date 03/2022
 */

#ifndef _COUCHE2_
#define _COUCHE2_

/**
 * @brief ecris le superblock en tout début du disque */
void write_super_block();

/**
 * @brief lis le superblock en tout début du disque */
void read_super_block();

/**
 * @brief update la position du premier octet libre à \a newpos
 * ne pas toucher sauf pour initialiser au premier demarage */
void update_first_free_byte(int newpos);

/**
 * @brief lis la table d'inode depuis le systeme */
void read_inodes_table();

/**
 * @brief ecris la table d'inode sur le systeme */
void write_inodes_table();

/**
 * @brief supprime l'inode a l'index \a index et compacte la table */
void delete_inode(int index);

/**
 * @return index du premier inode libre ou -1 si aucun de libre */
int get_unused_inode();

/**
 * @brief Initalise un inode avec quelques informations
 * @return 0>= OK, -1, si aucune place disponible */
int init_inode(char *filename, int pos, int size);

/**
 * @brief print la table d'inode */
void print_inode_table();

/**
 * @brief print les informations du superblock */
void print_superblock();

#endif
