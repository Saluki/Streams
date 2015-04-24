//
// Created by corentin on 20/04/15.
//

#ifndef STREAMS_LOG_H
#define STREAMS_LOG_H

// Log levels (following RFC2119)
#define LOG_EMERGENCY 0
#define LOG_ALERT 1
#define LOG_CRITICAL 2
#define LOG_ERROR 3
#define LOG_WARNING 4
#define LOG_NOTICE 5
#define LOG_INFO 6
#define LOG_DEBUG 7

// Log destinations
#define WRITE_OUT 0
#define WRITE_FILE 1

// Log path. Must end with a slash
#define LOG_FOLDER "/home/corentin/streams/"

int log_message(char* message, int log_level);
void set_log_method(char** argv);

#endif //STREAMS_LOG_H
