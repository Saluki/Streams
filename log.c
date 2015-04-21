//
// Created by corentin on 20/04/15.
//
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stdio.h>

#include "log.h"
#include "constants.h"

int log(char* message, int log_level)
{
    char* file_name;

    // check for folder, create otherwise
    mkdir(LOG_FOLDER, S_IRWXG);

    // extract current date
    time_t timestamp = time(NULL);
    struct tm *time_info = gmtime(&timestamp);
    sprintf(file_name, "log_%d_%d_%d", time_info->tm_yday, time_info->tm_mon, time_info->tm_year);

    // check if file, create otherwise

    // write to log

    // display if needed
    if(log_level<=DISPLAY_LEVEL)
    {
        fprintf(STDERR, "[LOG] %s\n", message);
    }
}
