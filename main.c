#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "constants.h"
#include "server.h"

int main(int argc, char** argv)
{
    /*int server_fd, port_number;
    char *end_ptr;

    if(argc!=2)
    {
        fprintf(stderr, "Incorrect usage\nUsage: %s [port-number]\n", *argv);
        exit(EXIT_FAILURE);
    }

    port_number = strtol(*(argv+1), &end_ptr, NUMERICAL_BASE);

    if(end_ptr!=NULL || port_number<=0)
    {
        fprintf(stderr, "Port number must be a positive integer\n");
        exit(EXIT_FAILURE);
    }

    server_fd = create_server(*(argv+1), 1);
    log("Streams server created", LOG_INFO);

    return 0;*/

    log_message("Server started", LOG_INFO);

    printf("Here we are\n");

    return 0;
}