/**
 * Corentin Badot-Bertrand cbadot-14
 * Corentin Dandoy cdandoy14
 *
 * Serveur STREAMS
 * Utilitaire permettant de créer le serveur et de le faire ecouter sur un port
 */
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "log.h"
#include "server.h"
#include "constants.h"

int create_server(int port_number, int max_connections)
{
    int server_fd, optval=1, bind_result, listen_result;
    struct sockaddr_in socket_details;

    // ETAPE 1
    // Creation du socket

    if( (server_fd=socket(PF_INET, SOCK_STREAM, 0)) == -1 )
    {
        log_error("Could not create server", LOG_CRITICAL, errno);
        exit(EXIT_FAILURE);
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));

    // ETAPE 2 "bind"
    // Affectation d'une adresse

    socket_details.sin_family = AF_INET;
    socket_details.sin_port = htons(port_number);
    socket_details.sin_addr.s_addr = htonl(INADDR_ANY);

    if( bind(server_fd, (struct sockaddr *) &socket_details, sizeof(struct sockaddr_in)) == -1 )
    {
        log_error("Could not bind server address", LOG_CRITICAL, errno);
        exit(EXIT_FAILURE);
    }

    // ETAPE 3 "listen"
    // Ecoute sur le ports

    if( listen(server_fd, 0) == -1 )
    {
        log_error("Could not listen on specified port", LOG_CRITICAL, errno);
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

void log_client_connection(int socket_fd)
{
    char message[MAX_ARRAY_SIZE];
    sprintf(message, "New connection with socket file descriptor #%d" ,socket_fd);

    log_message(message, LOG_INFO);
}