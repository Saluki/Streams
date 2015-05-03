/**
 * Corentin Badot-Bertrand cbadot-14
 * Corentin Dandoy cdandoy14
 *
 * Implémentation du serveur STREAMS.
 * Fichier contenant les définitions des fonctions de memory.c ainsi que des structures qui sont utilisées
 * dans la mémoire partagée.
 *
 * https://github.com/Saluki/Streams-Server
 */
#include <sys/types.h>
#include "constants.h"
#include "game.h"

#ifndef STREAMS_MEMORY_H
#define STREAMS_MEMORY_H

struct player {
    char name[MAX_USERNAME_LENGTH];
    int score;
    int fd;
};

struct memory {
    struct player players[MAX_NUMBER_USERS];
};

key_t get_shared_memory_token();
int create_shared_memory();
struct memory *attach_memory(int shmid);

#endif //STREAMS_MEMORY_H
