/**
 * Corentin Badot-Bertrand cbadot-14
 * Corentin Dandoy cdandoy14
 *
 * Serveur STREAMS
 * Fichier header contenant les définitions des fonctions de semaphore.c ainsi que des constantes
 * permettant d'identifier les sémaphores (MUTEX et ACCESS).
 *
 * https://github.com/Saluki/Streams-Server
 */
#include <sys/types.h>

#ifndef STREAMS_SEMAPHORE_H
#define STREAMS_SEMAPHORE_H

#define NB_SEMAPHORES 2
#define SINGLE_OPERATION 1

#define SEMAPHORE_MUTEX 0
#define SEMAPHORE_ACCESS 1

key_t get_semaphore_token();
void init_semaphores();
void delete_semaphores();
int semaphore_up(int semaphore);
int semaphore_down(int semaphore);

#endif //STREAMS_SEMAPHORE_H
