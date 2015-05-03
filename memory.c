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
#include <pwd.h>
#include <unistd.h>

#include "log.h"
#include "memory.h"

key_t get_shared_memory_token()
{
    char lock_file[MAX_ARRAY_SIZE];
    key_t semaphore_key;

    sprintf(lock_file,"%s/streams.lock", getpwuid(getuid())->pw_dir);

    if( (semaphore_key=ftok(lock_file, FTOK_SHARED_MEMORY_ID)) == -1 )
    {
        log_error("Could not create token using ftok()", LOG_ALERT, errno);
        exit(EXIT_FAILURE);
    }

    return semaphore_key;
}

int create_shared_memory() {

    int shmid;
    key_t key = get_shared_memory_token();

    if( (shmid=shmget(key, sizeof(struct memory), IPC_CREAT|0666)) < 0) {
        log_error("Error with shmget()", LOG_CRITICAL, errno);
        exit(EXIT_FAILURE);
    }

    return shmid;
}

struct memory* attach_memory(int shmid) {

    struct memory* shared_mem_ptr;

    if( (shared_mem_ptr=shmat(shmid, NULL, 0)) == (void *)-1 )
    {
        perror("shmat()");
        exit(EXIT_FAILURE);
    }

    return shared_mem_ptr;
}