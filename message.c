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
#include "message.h"
#include "log.h"

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

    struct message_t struct_message;
    enum message_type_t type = (enum message_type_t) (message[0] - '0');
    char *payload;

    switch (type) {

        case CANCEL_GAME:
            log_message("Cancelling game", LOG_INFO);
            break;

        case REGISTRATE:
            log_message("New client", LOG_INFO);

            size_t name_length;
            if ( (name_length = strlen(message)-1) < MIN_NAME_LENGTH) {
                perror("Client name invalid");
                exit(EXIT_FAILURE);
            }

            if ( (payload = malloc(name_length * sizeof(char))) == NULL) {
                perror("malloc()");
                exit(EXIT_FAILURE);
            }

            strncpy(payload, message+1, name_length);
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
            log_message("Bad message type", LOG_NOTICE);
            break;
    }

    struct_message.type = type;
    struct_message.payload = payload;
    return struct_message;
}