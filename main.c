/**
 * Corentin Badot-Bertrand cbadot-14
 * Corentin Dandoy cdandoy14
 *
 * Implémentation du serveur STREAMS
 * Point d'entrée de l'application serveur et boucle principale du serveur.
 *
 * https://github.com/Saluki/Streams-Server
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <sys/shm.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#include "log.h"
#include "lock.h"
#include "constants.h"
#include "server.h"
#include "game.h"
#include "message.h"
#include "memory.h"
#include "semaphore.h"

void register_signal_handlers();

void sig_end_handler(int signal_number);

void sig_alarm_handler(int i);

void check_incorrect_usage(int argc, char **argv);

int extract_port_number(char **argv);

int shmid;
struct memory *shared_mem_ptr;

int main(int argc, char **argv) {

    int server_fd, port_number, max_fd, file_descriptors[MAX_NUMBER_USERS], i;
    int temp_fd, select_result, timer_is_active = FALSE, status_code;

    char *validation;

    fd_set file_descriptor_set;

    check_incorrect_usage(argc, argv);
    set_log_method(argv);
    set_lock();

    port_number = extract_port_number(argv);
    server_fd = create_server(port_number, MAX_NUMBER_USERS);
    log_message("Streams server created", LOG_INFO);

    register_signal_handlers();

    for (i = 0; i < MAX_NUMBER_USERS; i++) {
        file_descriptors[i] = 0;
    }

    // -- SHARED MEMORY AND SEMAPHORES --

    shmid = create_shared_memory();
    shared_mem_ptr = attach_memory(shmid);
    init_semaphores();

    log_message("Shared memory and semaphores created", LOG_DEBUG);

    // -- SERVER LOOP --

    while (TRUE) {

        FD_ZERO(&file_descriptor_set);
        FD_SET(server_fd, &file_descriptor_set);
        max_fd = server_fd;

        for (i = 0; i < MAX_NUMBER_USERS; i++) {
            temp_fd = file_descriptors[i];

            if (temp_fd > 0) {
                FD_SET(temp_fd, &file_descriptor_set);
            }

            if (temp_fd > max_fd) {
                max_fd = temp_fd;
            }
        }

        select_result = select(max_fd + 1, &file_descriptor_set, NULL, NULL, NULL);

        if (select_result < 0 && errno != EINTR) {
            log_error("Select call error", LOG_WARNING, errno);
            continue;
        }

        if (FD_ISSET(server_fd, &file_descriptor_set)) {

            if ((temp_fd = accept(server_fd, NULL, 0)) < 0) {
                log_error("Could not accept incoming connection", LOG_ALERT, errno);
                exit(EXIT_FAILURE);
            }

            log_client_connection(temp_fd);

            for (i = 0; i < MAX_NUMBER_USERS; i++) {
                if (file_descriptors[i] != 0)
                    continue;

                file_descriptors[i] = temp_fd;
                break;
            }
        }

        for (i = 0; i < MAX_NUMBER_USERS; i++) {

            temp_fd = file_descriptors[i];

            if (!FD_ISSET(temp_fd, &file_descriptor_set))
                continue;

            char *message = NULL;
            if ((message = (char *) calloc(MESSAGE_LENGTH, sizeof(char))) == NULL) {
                log_error("Memory allocation error", LOG_ALERT, errno);
                exit(EXIT_FAILURE);
            }

            if (recv(temp_fd, message, MESSAGE_LENGTH, 0) <= 0)   // Disconnected
            {
                log_message("Client disconnected", LOG_INFO);

                close(temp_fd);
                file_descriptors[i] = 0;
            }
            else    // Message sent to server
            {
                struct message_t mess=decode(message);

                if( (status_code = mess.type) == ERROR_MESSAGE) {
                    continue;
                }

                if (get_game_phase() == REGISTER_PHASE) {

                    if (status_code != 1) {
                        // TODO Send message back to user?
                        log_message("Currently register phase. User can only register", LOG_DEBUG);
                        continue;
                    }

                    if (!timer_is_active) {
                        log_message("Starting register timer", LOG_DEBUG);
                        alarm(WAIT_TIME);
                        timer_is_active = TRUE;
                    }

                    char *new_user = (char *) malloc(MAX_ARRAY_SIZE * sizeof(char));
                    sprintf(new_user, "User '%s' asks for registration. Adding user in memory.", (char *) mess.payload);
                    log_message(new_user, LOG_INFO);

                    // Add a player to the shared memory
                    semaphore_down(SEMAPHORE_ACCESS);
                    strncpy(shared_mem_ptr->players->name, (char *) mess.payload, strlen(mess.payload));
                    shared_mem_ptr->players[i].fd = temp_fd;
                    shared_mem_ptr->players[i].score = 15; // TODO A supprimer
                    semaphore_up(SEMAPHORE_ACCESS);

                    validation = encode(VALID_REGISTRATION, "1");
                    send(temp_fd, validation, strlen(validation), 0);
                }
                else    // GAME PHASE
                {
                    log_message("Game phase. Not yet implemented.", LOG_INFO);
                }

            }
        }
    }

    return 0;
}

void register_signal_handlers() {

    if (signal(SIGINT, sig_end_handler) == SIG_ERR || signal(SIGTERM, sig_end_handler) == SIG_ERR) {
        log_error("Cannot listen to SIGINT/SIGTERM signal", LOG_CRITICAL, errno);
        exit(EXIT_FAILURE);
    }

    if (signal(SIGALRM, sig_alarm_handler) == SIG_ERR) {
        log_error("Cannot listen to SIGALRM signal", LOG_CRITICAL, errno);
        exit(EXIT_FAILURE);
    }
}

void sig_end_handler(int signal_number) {

    if (signal_number == SIGINT || signal_number == SIGTERM) {

        shmdt(shared_mem_ptr);
        shmctl(shmid, IPC_RMID, NULL);
        log_message("Shared memory deleted from system", LOG_DEBUG);

        delete_semaphores();

        remove_lock();
        log_message("Shutting down streams server", LOG_INFO);
        exit(EXIT_SUCCESS);
    }
}

void sig_alarm_handler(int signal_number) {

    set_game_phase(GAME_PHASE);
    log_message("Timer finished", LOG_DEBUG);
}

void check_incorrect_usage(int argc, char **argv) {

    if (argc != 3) {
        fprintf(stderr, "Incorrect usage\nUsage: %s [port-number] [out|file]\n", *argv);
        exit(EXIT_FAILURE);
    }
}

int extract_port_number(char **argv) {

    int port_number;
    char *end_ptr;

    port_number = (int) strtol(*(argv + 1), &end_ptr, NUMERICAL_BASE);

    if (*end_ptr != '\0' || port_number <= 0) {
        fprintf(stderr, "Port number must be a positive integer\n");
        exit(EXIT_FAILURE);
    }

    return port_number;
}