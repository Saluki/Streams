/*
 * BADOT-BERTRAND Corentin cbadot-14
 * DANDOY Corentin cdandoy14
 *
 * Fonctions liées à la mémoire partagée
 */

#include "constants.h"

#ifndef STREAMS_MEMORY_H
#define STREAMS_MEMORY_H

struct memory {
    struct player player[MAX_NUMBER_USERS];
};

struct player {
    char name[MAX_USERNAME_LENGTH];
    int fd;
};

int create_mem();
struct memory* attach_mem(int shmid);

#endif //STREAMS_MEMORY_H
