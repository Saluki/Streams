//
// Created by corentin on 24/04/15.
//

#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "lock.h"
#include "log.h"
#include "constants.h"

int set_lock()
{
    char lock_file[MAX_ARRAY_SIZE];
    int lock_fd;

    // Lock file
    const char *home_dir = getpwuid(getuid())->pw_dir;
    sprintf(lock_file,"%s/streams.lock", home_dir);

    // Test access to lock file, quit if necessary
    if( access(lock_file, F_OK)==0 ) {
        log_message("Server instance already exists", LOG_WARNING);
        exit(EXIT_FAILURE);
    }

    // Create lock file
    if ((lock_fd = open(lock_file, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IROTH)) == -1) {
        log_message("Could not create lock file", LOG_ALERT);
        exit(EXIT_FAILURE);
    }

    log_message("Created lock file in home directory", LOG_DEBUG);
}

int remove_lock()
{
    char lock_file[MAX_ARRAY_SIZE];

    // Lock file
    const char *home_dir = getpwuid(getuid())->pw_dir;
    sprintf(lock_file,"%s/streams.lock", home_dir);

    if( unlink(lock_file)==-1 )
    {
        log_message("Could not delete lock file in home directory", LOG_ALERT);
    }
    else
    {
        log_message("Lock file removed", LOG_DEBUG);
    }
}