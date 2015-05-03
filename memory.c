/*
 * BADOT-BERTRAND Corentin cbadot-14
 * DANDOY Corentin cdandoy14
 *
 * Fonctions liées à la mémoire partagée
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "log.h"
#include "memory.h"

int create_mem() {

    int shmid;
    key_t key = ftok(PATH_NAME, PROJECT_ID);

    if( (shmid=shmget(key, sizeof(struct memory), IPC_CREAT|0666)) < 0) {
        log_error("Error with shmget()", LOG_CRITICAL, errno);
        exit(EXIT_FAILURE);
    }

    return shmid;
}

struct memory* attach_mem(int shmid) {

    struct memory* shared_mem_ptr;

    if( (shared_mem_ptr=shmat(shmid, NULL, 0)) == (void *)-1 )
    {
        perror("shmat()");
        exit(EXIT_FAILURE);
    }

    return shared_mem_ptr;
}