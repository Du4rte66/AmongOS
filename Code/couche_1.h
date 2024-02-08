/**
 * @file couche_1.c
 * @brief contient l'interface de la couche 1
 * @author romeo,duarte,chedli,elyes
 * @date 13/03/2022
 */
#ifndef _COUCHE1_
#define _COUCHE1_

/**
 * @brief initialise la variable gloable representant le disque
 */
void init_disk_os(const char *dirname);

/**
 * @return n/BLOCK_SIZE(+1 if %!=0);
 */
size_t compute_nblock(size_t n);

/**
 * @brief lis \a taille octets à la position \a pos et stocke les informations\
 * dans \a buffer
 * @return 1 si erreur, 0 sinon */
int read_block(void *buffer, long pos, size_t taille);

/**
 * @brief ecris \a taille octets à la position \a pos depuis les informations\
 * dans \a buffer
 * @return 1 si erreur, 0 sinon */
int write_block(void *buffer, long pos, size_t taille);

/**
 * @brief Eteint le systeme \
 * (ecrit une derniere fois la variable dans le fichier et ferme)
 */
void shutDown_disk_os();

#endif
