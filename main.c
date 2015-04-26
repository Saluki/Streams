/**
 * Corentin Badot-Bertrand cbadot-14
 * Corentin Dandoy cdandoy14
 *
 * Serveur STREAMS
 * Point d'entrée de l'application et boucle principale du serveur
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include "log.h"
#include "lock.h"
#include "constants.h"
#include "server.h"
#include "game.h"
#include "message.h"

void sig_handler(int signal_number);
void sig_alarm_handler(int i);
void check_incorrect_usage(int argc, char** argv);
int extract_port_number(char** argv);

int main(int argc, char** argv)
{
    int server_fd, client_fd, port_number, max_fd, file_descriptors[MAX_NUMBER_USERS], i;
    int temp_fd, select_result, nb_bytes_read, timer_is_active=FALSE, status_code;

    char buffer[MAX_ARRAY_SIZE];
    char *end_ptr, *msg_ptr;
    char register_confirm_msg[] = "Registration complete\n";

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

        select_result = select(max_fd+1, &file_descriptor_set, NULL , NULL , NULL);

        if( select_result<0 && errno!=EINTR )
        {
            log_message("select() error", LOG_WARNING);
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
                    // PROCESSING REQUEST

                    if( get_game_phase() == REGISTER_PHASE )
                    {
                        status_code =  strtol(buffer, &end_ptr, NUMERICAL_BASE);

                        if( status_code == 1 )
                        {
                            if(!timer_is_active)
                            {
                                log_message("Starting register timer", LOG_DEBUG);
                                alarm(WAIT_TIME);
                                timer_is_active = TRUE;
                            }

                            log_message("User asks for registration. Adding user in memory.", LOG_INFO);
                            msg_ptr = (char*) encode(VALID_REGISTRATION, "1");

                            send(temp_fd, msg_ptr, strlen(msg_ptr), 0);
                        }
                    }
                    else
                    {
                        // GAME PHASE
                        log_message("Game phase. Not yet implemented.", LOG_INFO);
                    }
                }
            }
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

void sig_alarm_handler(int i)
{
    set_game_phase(GAME_PHASE);
    log_message("Timer finished", LOG_DEBUG);
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