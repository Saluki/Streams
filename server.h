//
// Created by corentin on 20/04/15.
//

#ifndef STREAMS_SERVER_H
#define STREAMS_SERVER_H

#define DEFAULT_NUMBER_USERS 2

struct client_node
{
    struct client_node *next;
    int client_fd;
};

int create_server(int port_number, int max_connections);
int add_client(int client_fd);
void display_clients();

#endif //STREAMS_SERVER_H
