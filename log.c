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

// TODO
// - Add hour/date to log file
// - Debug
// - Find home directory
int log_message(char* message, int log_level)
{
    int log_fd;
    char file_name[MAX_ARRAY_SIZE], file_path[MAX_ARRAY_SIZE], log_message[MAX_ARRAY_SIZE];

    // check for folder, create otherwise
    // TODO Check only for correct permissions (filter error msg)
    mkdir(LOG_FOLDER, S_IRWXU | S_IRWXG | S_IROTH); // All rights for user/group + read for others

    strcpy(file_path, LOG_FOLDER);

    time_t timestamp = time(NULL);
    struct tm *time_info = gmtime(&timestamp);
    sprintf(file_name, "log_%d_%d_%d.log", time_info->tm_mday+1, time_info->tm_mon+1, time_info->tm_year+1900);

    strcat(file_path,file_name);

    if( (log_fd=open(file_path, O_WRONLY | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IROTH)) == -1 )
    {
        perror("Could not open log file");
        return -1;
    }

    sprintf(log_message, "[LOG #%d] %s\n", log_level, message);

    if( (write(log_fd,log_message,strlen(log_message))) == -1 )
    {
        perror("Could not write in log file");
        return -1;
    }

    // display if needed
    if(log_level<=DISPLAY_LEVEL)
    {
        //fprintf(STDERR, "[LOG] %s\n", message);
        printf("Hey...\n");
    }

    return 0;
}
