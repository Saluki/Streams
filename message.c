/**
 * Corentin Badot-Bertrand cbadot-14
 * Corentin Dandoy cdandoy14
 *
 * Serveur STREAMS
 * Fonctions permettant d'encoder et de decoder des messages pour le client
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "message.h"
#include "log.h"

// TODO Check if MESSAGE LENGTH must be 26 -> terminating \0
char* encode(enum message_type_t type, void *payload)
{
    char *message;
    if ( (message = malloc(MESSAGE_LENGTH * sizeof(char))) == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    sprintf(message, "%d%s\n", type, (char*) payload);
    return message;
}

struct message_t decode(char* message) {

    int name_length;
    char *payload;
    struct message_t struct_message;
    enum message_type_t type;

    if( strlen(message)<2 )
    {
        log_message("Message length is too short", LOG_DEBUG);

        struct_message.type = -1;
        struct_message.payload = NULL;
        return struct_message;
    }

    type = (enum message_type_t) (message[0] - '0');
    message++;

    switch (type) {

        case CANCEL_GAME:
            log_message("Cancelling game", LOG_INFO);
            break;

        case REGISTRATE:

            // Removing \r and \n characters
            message = strtok(message, "\r\n");

            name_length = strlen(message);

            if( (payload=(char*)malloc(name_length*sizeof(char))) == NULL)
            {
                log_error("Memory allocation error. Could not allow memory for payload", LOG_ALERT, errno);
                exit(EXIT_FAILURE);
            }

            strncpy(payload, message, name_length+1);
            break;

        case VALID_REGISTRATION:
            log_message("Valid registration", LOG_INFO);
            if ( (payload = malloc(1 * sizeof(char))) == NULL) {
                perror("malloc()");
                exit(EXIT_FAILURE);
            }
            strncpy(payload, message+1, 1);
            break;

        case NEW_PICK:
            log_message("New pick", LOG_INFO);
            break;

        case CHOOSE_PICK:
            log_message("Position chosen", LOG_INFO);
            break;

        case END_GAME:
            log_message("End of game", LOG_INFO);
            break;

        default:

            type = -1;
            log_message("Incorrect message format: wrong type", LOG_NOTICE);
            break;
    }

    struct_message.type = type;
    struct_message.payload = payload;
    return struct_message;
}