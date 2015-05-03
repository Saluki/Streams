//
// Created by corentin on 03/05/15.
//

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>

#include "constants.h"
#include "log.h"
#include "semaphore.h"

int semaphore_fd = -1;

key_t get_semaphore_token()
{
    char lock_file[MAX_ARRAY_SIZE];
    key_t semaphore_key;

    sprintf(lock_file,"%s/streams.lock", getpwuid(getuid())->pw_dir);

    if( (semaphore_key=ftok(lock_file, FTOK_SEMAPHORES_ID)) == -1 )
    {
        log_error("Could not create token using ftok()", LOG_ALERT, errno);
        exit(EXIT_FAILURE);
    }

    return semaphore_key;
}

void init_semaphores()
{
    key_t semaphore_key = get_semaphore_token();

    if( (semaphore_fd=semget(semaphore_key, NB_SEMAPHORES, IPC_CREAT|0666)) == -1 )
    {
        log_error("Could not create semaphore using semget()", LOG_CRITICAL, errno);
        exit(EXIT_FAILURE);
    }

    if( semctl(semaphore_fd, SEMAPHORE_MUTEX, SETVAL, 1) == -1 || semctl(semaphore_fd, SEMAPHORE_ACCESS, SETVAL, 1) == -1 )
    {
        log_error("Could not initialize semaphores", LOG_CRITICAL, errno);
        exit(EXIT_FAILURE);
    }
}

void delete_semaphores()
{
    if( semctl(semaphore_fd, 0, IPC_RMID)==-1 )
    {
        log_error("Could not delete semaphore set", LOG_CRITICAL, errno);
    }

    log_message("Semaphores deleted from system", LOG_DEBUG);
}

int semaphore_up(int semaphore)
{
    struct sembuf operations_buffer[1], semaphore_operation;

    if( semaphore_fd == -1 )
    {
        log_message("Semaphore set not created", LOG_WARNING);
        return -1;
    }

    semaphore_operation.sem_num = semaphore;
    semaphore_operation.sem_op = 1;
    semaphore_operation.sem_flg = 0;
    operations_buffer[0] = semaphore_operation;

    if( semop(semaphore_fd, operations_buffer, SINGLE_OPERATION) == -1 )
    {
        log_error("Could not perform operation up() on semaphore", LOG_ALERT, errno);
        return -1;
    }

    return 0;
}

int semaphore_down(int semaphore)
{
    struct sembuf operations_buffer[1], semaphore_operation;

    if( semaphore_fd == -1 )
    {
        log_message("Semaphore set not created", LOG_WARNING);
        return -1;
    }

    semaphore_operation.sem_num = semaphore;
    semaphore_operation.sem_op = -1;
    semaphore_operation.sem_flg = 0;
    operations_buffer[0] = semaphore_operation;

    if( semop(semaphore_fd, operations_buffer, SINGLE_OPERATION) == -1 )
    {
        log_error("Could not perform operation down() on semaphore", LOG_ALERT, errno);
        return -1;
    }

    return 0;
}

