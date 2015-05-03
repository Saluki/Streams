/**
 * Corentin Badot-Bertrand cbadot-14
 * Corentin Dandoy cdandoy14
 *
 * Serveur STREAMS
 * Fichier header contenant les définitions des fonctions de serveur.c.
 *
 * https://github.com/Saluki/Streams-Server
 */
#ifndef STREAMS_SERVER_H
#define STREAMS_SERVER_H

int create_server(int port_number, int max_connections);
void log_client_connection(int fd);

#endif //STREAMS_SERVER_H
