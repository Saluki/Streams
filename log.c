//
// Created by corentin on 20/04/15.
//
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "log.h"
#include "constants.h"

int current_log_method = WRITE_OUT;

int log_message(char* message, int log_level)
{
    time_t timestamp = time(NULL);
    struct tm *time_info = gmtime(&timestamp);

    if(current_log_method==WRITE_FILE)
    {
        int log_fd;
        char file_name[MAX_ARRAY_SIZE], file_path[MAX_ARRAY_SIZE], log_message[MAX_ARRAY_SIZE];

        mkdir(LOG_FOLDER, S_IRWXU | S_IRWXG | S_IROTH); // All rights for user/group + read for others

        sprintf(file_name, "log_%d_%d_%d.log", time_info->tm_mday + 1, time_info->tm_mon + 1,
                time_info->tm_year + 1900);

        strcpy(file_path, LOG_FOLDER);
        strcat(file_path, file_name);

        if ((log_fd = open(file_path, O_WRONLY | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IROTH)) == -1) {
            perror("Could not open log file");
            return -1;
        }

        sprintf(log_message, "[LOG #%d %d:%d:%d] %s\n", log_level, time_info->tm_hour, time_info->tm_min, time_info->tm_sec, message);

        if ((write(log_fd, log_message, strlen(log_message))) == -1) {
            perror("Could not write in log file");
            return -1;
        }

        close(log_fd);
    }
    else
    {
        printf("[LOG #%d %d/%d/%d %d:%d:%d] %s\n", log_level, time_info->tm_mday + 1, time_info->tm_mon + 1,
               time_info->tm_year + 1900, time_info->tm_hour, time_info->tm_min, time_info->tm_sec, message);
    }

    return 0;
}

void set_log_method(char** argv)
{
    if( strcmp("out",*(argv+2))==0 ) {
        current_log_method = WRITE_OUT;
        printf("[LOG -> OUT]\n");
    }
    else if( strcmp("file",*(argv+2))==0 ) {
        current_log_method = WRITE_FILE;
        printf("[LOG -> FILE]\n");
    }
    else {
        fprintf(stderr, "Incorrect usage\nUsage: %s [port-number] [out|file]\n", *argv);
        exit(EXIT_FAILURE);
    }
}
