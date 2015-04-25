#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>

#include "log.h"
#include "lock.h"
#include "constants.h"
#include "server.h"
#include "game.h"

void sig_handler(int signal_number);
void check_incorrect_usage(int argc, char** argv);
int extract_port_number(char** argv);

int main(int argc, char** argv)
{
    int server_fd, client_fd, port_number;

    check_incorrect_usage(argc, argv);
    set_log_method(argv);
    set_lock();

    port_number = extract_port_number(argv);
    server_fd = create_server(port_number, DEFAULT_NUMBER_USERS);
    log_message("Streams server created", LOG_INFO);

    if( signal(SIGINT, sig_handler)==-1 ||  signal(SIGTERM, sig_handler)==-1 )
    {
        log_message("Cannot listen to SIGINT/SIGTERM signal", LOG_CRITICAL);
        exit(EXIT_FAILURE);
    }

    while(TRUE)
    {
        if ((client_fd = accept(server_fd, NULL, 0)) == -1)
        {
            log_message("Could not open accept client", LOG_ALERT);
            continue;
        }

        if( get_game_phase()==REGISTER_PHASE )
        {
            add_client(client_fd);

            char msg_client[] = "Hello World...\nThis is a message from the Streams Server";
            write(client_fd, msg_client, strlen(msg_client));
        }
    }

    return 0;
}

void sig_handler(int signal_number)
{
    if(signal_number == SIGINT || signal_number == SIGTERM)
    {
        remove_lock();
        log_message("Shutting down streams server", LOG_INFO);
        exit(EXIT_SUCCESS);
    }
}

void check_incorrect_usage(int argc, char** argv)
{
    if(argc!=3)
    {
        fprintf(stderr, "Incorrect usage\nUsage: %s [port-number] [out|file]\n", *argv);
        exit(EXIT_FAILURE);
    }
}

int extract_port_number(char** argv)
{
    int port_number;
    char *end_ptr;

    port_number = strtol(*(argv+1), &end_ptr, NUMERICAL_BASE);

    if(*end_ptr!='\0' || port_number<=0)
    {
        fprintf(stderr, "Port number must be a positive integer\n");
        exit(EXIT_FAILURE);
    }

    return port_number;
}