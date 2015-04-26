#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>
#include <netinet/in.h>

#include "log.h"
#include "lock.h"
#include "constants.h"
#include "server.h"
#include "game.h"

void sig_handler(int signal_number);
void sig_alarm_handler(int i);
void check_incorrect_usage(int argc, char** argv);
int extract_port_number(char** argv);

int main(int argc, char** argv)
{
    int server_fd, client_fd, port_number, max_fd, file_descriptors[MAX_NUMBER_USERS], i, temp_fd, nb_bytes_read;
    char buffer[MAX_ARRAY_SIZE];
    fd_set file_descriptor_set;

    check_incorrect_usage(argc, argv);
    set_log_method(argv);
    set_lock();

    port_number = extract_port_number(argv);
    server_fd = create_server(port_number, MAX_NUMBER_USERS);
    log_message("Streams server created", LOG_INFO);

    if( signal(SIGINT, sig_handler)==-1 ||  signal(SIGTERM, sig_handler)==-1 )
    {
        log_message("Cannot listen to SIGINT/SIGTERM signal", LOG_CRITICAL);
        exit(EXIT_FAILURE);
    }

    if( signal(SIGALRM, sig_alarm_handler)==-1 )
    {
        log_message("Cannot listen to SIGALRM signal", LOG_CRITICAL);
        exit(EXIT_FAILURE);
    }

    for (i=0; i<MAX_NUMBER_USERS; i++)
    {
        file_descriptors[i] = 0;
    }

    while(TRUE)
    {
        FD_ZERO(&file_descriptor_set);
        FD_SET(server_fd, &file_descriptor_set);
        max_fd = server_fd;

        for(i=0; i<MAX_NUMBER_USERS; i++)
        {
            temp_fd = file_descriptors[i];

            if(temp_fd>0)
            {
                FD_SET(temp_fd, &file_descriptor_set);
            }

            if(temp_fd>max_fd)
            {
                max_fd = temp_fd;
            }
        }

        if( select(max_fd+1, &file_descriptor_set, NULL , NULL , NULL)==-1 )
        {
            log_message("select() error", LOG_ALERT);
            continue;
        }

        if( FD_ISSET(server_fd, &file_descriptor_set) )
        {
            if( (temp_fd=accept(server_fd, NULL, 0)) < 0 )
            {
                log_message("Could not accept incoming connection", LOG_ALERT);
                exit(EXIT_FAILURE);
            }

            log_client_connection(temp_fd);

            for(i=0; i<MAX_NUMBER_USERS; i++)
            {
                if( file_descriptors[i] != 0 )
                    continue;

                file_descriptors[i] = temp_fd;
                break;
            }
        }

        for(i=0; i<MAX_NUMBER_USERS; i++)
        {
            temp_fd = file_descriptors[i];

            if( FD_ISSET(temp_fd, &file_descriptor_set) )
            {
                if ((nb_bytes_read = read(temp_fd, buffer, MAX_ARRAY_SIZE)) == 0)
                {
                    log_message("Client disconnected", LOG_INFO);

                    close(temp_fd);
                    file_descriptors[i] = 0;
                }
                else
                {
                    // PROCESSING TRANSACTION

                    buffer[nb_bytes_read] = '\0';
                    send(temp_fd, buffer, strlen(buffer), 0);
                }
            }
        }
    }

    return 0;
}

void sig_handler(int signal_number)
{
    // TODO Clean IPC's
    // TODO Use sigaction() ?

    if(signal_number == SIGINT || signal_number == SIGTERM)
    {
        remove_lock();
        log_message("Shutting down streams server", LOG_INFO);
        exit(EXIT_SUCCESS);
    }
}

void sig_alarm_handler(int i)
{
    log_message("Alarm was fired", LOG_DEBUG);
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