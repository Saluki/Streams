//
// Created by coco on 26/04/15.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "message.h"
#include "log.h"

char* encode(enum message_type_t type, void* payload)
{
    char *message;

    if( (message=(char*)malloc(MESSAGE_LENGTH * sizeof(char))) == NULL )
        log_message("Could not allocate memory for encode()", LOG_CRITICAL);
        exit(EXIT_FAILURE);
    }

    message[0] = (char) type;

    strncpy(message+1, payload, MESSAGE_LENGTH-1 * sizeof(char));

    return message;
}

void* decode(char* message) {

    void *payload = malloc((MESSAGE_LENGTH - 1) * sizeof(char));

    enum message_type_t type = message[0];
    switch (type) {
        case CANCEL_GAME:
            log_message("Cancelling game", LOG_INFO);
            break;
        case REGISTRATE:
            log_message("New client", LOG_INFO);
            strncpy(payload, message + 1, MESSAGE_LENGTH - 1);
            break;
        case VALID_REGISTRATION:
            log_message("Valid registration", LOG_INFO);
            strncpy(payload, message + 1, 1);
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

    return payload;
}