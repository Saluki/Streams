/**
 * Corentin Badot-Bertrand cbadot-14
 * Corentin Dandoy cdandoy14
 *
 * Implémentation du serveur STREAMS.
 * Fonctions permettant à l'application de logger des messages et des erreurs avec précision.
 *
 * https://github.com/Saluki/Streams-Server
 */
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>

#include "log.h"
#include "constants.h"

char* create_log_message(struct tm *time_info, int log_level, char *message, int errno_number);

int current_log_method = WRITE_OUT;

int log_message(char* message, int log_level)
{
    log_error(message, log_level, -1);
}

int log_error(char* message, int log_level, int errno_number)
{
    int log_fd = 1;
    char *log_msg_ptr, file_path[MAX_ARRAY_SIZE], *home_dir;

    time_t timestamp = time(NULL);
    struct tm *time_info = gmtime(&timestamp);

    if(current_log_method==WRITE_FILE)
    {
        home_dir = getpwuid(getuid())->pw_dir;

        sprintf(file_path, "%s/%s", home_dir, LOG_FOLDER);
        mkdir(file_path, S_IRWXU | S_IRWXG | S_IROTH); // All rights for user/group + read for others

        sprintf(file_path, "%s/%s/log_%d_%d_%d.log", home_dir, LOG_FOLDER, time_info->tm_mday + 1, time_info->tm_mon + 1,
                time_info->tm_year + 1900);

        if ((log_fd = open(file_path, O_WRONLY | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IROTH)) == -1)
        {
            perror("Could not open log file");
            return -1;
        }
    }

    log_msg_ptr = create_log_message(time_info, log_level, message, errno_number);

    if(write(log_fd, log_msg_ptr, strlen(log_msg_ptr)) == -1)
    {
        perror("Could not write in log file");
        return -1;
    }

    free(log_msg_ptr);

    if(current_log_method==WRITE_FILE)
    {
        close(log_fd);
    }

    return 0;
}

// TODO Hour-and-day-offset bug. Something with the timezone?
char* create_log_message(struct tm *time_info, int log_level, char *message, int errno_number)
{
    char *log_ptr;

    if( (log_ptr=(char*)malloc(MAX_ARRAY_SIZE* sizeof(char))) == NULL )
    {
        perror("malloc() error");
        exit(EXIT_FAILURE);
    }

    if( errno_number<0 )
    {
        sprintf(log_ptr, "[LOG #%d %d/%d/%d %d:%d:%d] %s\n", log_level, time_info->tm_mday + 1, time_info->tm_mon + 1,
                time_info->tm_year + 1900, time_info->tm_hour, time_info->tm_min, time_info->tm_sec, message);
    }
    else
    {
        sprintf(log_ptr, "[LOG #%d %d/%d/%d %d:%d:%d] %s : %s\n", log_level, time_info->tm_mday + 1, time_info->tm_mon + 1,
                time_info->tm_year + 1900, time_info->tm_hour, time_info->tm_min, time_info->tm_sec, message, strerror(
                        errno_number));
    }

    return log_ptr;
}

void set_log_method(char** argv)
{
    if( strcmp("out",*(argv+2))==0 )
    {
        current_log_method = WRITE_OUT;
        printf("[LOG] Writing logs to stdout\n");
    }
    else if( strcmp("file",*(argv+2))==0 )
    {
        current_log_method = WRITE_FILE;
        printf("[LOG] Writing logs to file\n");
    }
    else
    {
        fprintf(stderr, "Incorrect usage\nUsage: %s [port-number] [out|file]\n", *argv);
        exit(EXIT_FAILURE);
    }
}
