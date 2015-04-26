//
// Created by corentin on 20/04/15.
//

#ifndef STREAMS_SERVER_H
#define STREAMS_SERVER_H

#define MAX_NUMBER_USERS 10

int create_server(int port_number, int max_connections);
void log_client_connection(int fd);

#endif //STREAMS_SERVER_H
