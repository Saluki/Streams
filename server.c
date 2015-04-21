//
// Created by corentin on 20/04/15.
//

#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

#include "server.h"

int create_server(int port_number, int max_connections)
{
    int server_fd, optval=1, bind_result, listen_result;
    struct sockaddr_in socket_details;

    // ETAPE 1
    // Creation du socket

    if( (server_fd=socket(PF_INET, SOCK_STREAM, 0)) == -1 )
    {
        fprintf(stderr, "Could not create server\n");
        exit(EXIT_FAILURE);
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));

    // ETAPE 2 "bind"
    // Affectation d'une adresse

    socket_details.sin_family = AF_INET;
    socket_details.sin_port = htons(port_number);
    socket_details.sin_addr.s_addr = htonl(INADDR_ANY);

    bind_result = bind(server_fd, (struct sockaddr *) &socket_details, sizeof(struct sockaddr_in));

    if( bind_result == -1 )
    {
        exit(EXIT_FAILURE);
    }

    // ETAPE 3 "listen"
    // Ecoute sur le ports

    listen_result = listen(server_fd, 0);

    if( listen_result == -1 )
    {
        exit(EXIT_FAILURE);
    }

    return server_fd;
}