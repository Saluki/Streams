//
// Created by corentin on 20/04/15.
//

#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

#include "log.h"
#include "server.h"

struct client_node *client_chain_begin=NULL, *client_chain_end=NULL;

/*shutdown(client_fd, 2);
close(client_fd);*/

int create_server(int port_number, int max_connections)
{
    int server_fd, optval=1, bind_result, listen_result;
    struct sockaddr_in socket_details;

    // ETAPE 1
    // Creation du socket

    if( (server_fd=socket(PF_INET, SOCK_STREAM, 0)) == -1 )
    {
        log_message("Could not create server", LOG_CRITICAL);
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
        log_message("Could not bind server address", LOG_CRITICAL);
        exit(EXIT_FAILURE);
    }

    // ETAPE 3 "listen"
    // Ecoute sur le ports

    listen_result = listen(server_fd, 0);

    if( listen_result == -1 )
    {
        log_message("Could not listen on specified port", LOG_CRITICAL);
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

int add_client(int client_fd)
{
    struct client_node *node;
    if( (node=(struct client_node*)malloc(sizeof(struct client_node))) == -1 )
    {
        log_message("Could not allow memory for client node", LOG_WARNING);
        return -1;
    }

    log_message("Client structure created. Adding to list.", LOG_DEBUG);

    node->client_fd = client_fd;
    node->next = NULL;

    if( client_chain_begin==NULL && client_chain_end==NULL )
    {
        client_chain_begin = node;
    }
    else
    {
        client_chain_end->next = node;
    }
    client_chain_end = node;

    log_message("Client added to list", LOG_DEBUG);

    return 0;
}

void display_clients()
{
    struct client_node *ptr = client_chain_begin;

    if( client_chain_begin==NULL && client_chain_end==NULL )
    {
        log_message("No clients in structure", LOG_DEBUG);
        return;
    }

    while(ptr!=NULL)
    {
        printf("--> client");
        ptr = ptr->next;
    }

    printf("\n");
}