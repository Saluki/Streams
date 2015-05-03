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
    int port;
    char name[MAX_USERNAME_LENGTH];
    int fd;
};

#endif //STREAMS_MEMORY_H
