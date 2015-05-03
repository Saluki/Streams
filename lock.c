/**
 * Corentin Badot-Bertrand cbadot-14
 * Corentin Dandoy cdandoy14
 *
 * Implémentation du serveur STREAMS.
 * Ces fonctions ont pour but de pouvoir mettre un "lock" sur le serveur, c'est à dire,
 * d'offrir un mécanisme qui n'autorise qu'une seule instance du serveur streams sur le système.
 *
 * https://github.com/Saluki/Streams-Server
 */
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

#include "lock.h"
#include "log.h"
#include "constants.h"

int set_lock()
{
    char lock_file[MAX_ARRAY_SIZE];

    // Lock file
    const char *home_dir = getpwuid(getuid())->pw_dir;
    sprintf(lock_file,"%s/streams.lock", home_dir);

    // Test access to lock file, quit if necessary
    if( access(lock_file, F_OK)==0 )
    {
        log_message("Server instance already exists. If not, manually delete the streams lock file.", LOG_WARNING);
        exit(EXIT_FAILURE);
    }

    // Create lock file
    if ((open(lock_file, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IROTH)) == -1)
    {
        log_error("Could not create lock file", LOG_ALERT, errno);
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
        log_error("Could not delete lock file in home directory", LOG_ALERT, errno);
        return 1;
    }

    log_message("Lock file removed", LOG_DEBUG);
    return 0;
}