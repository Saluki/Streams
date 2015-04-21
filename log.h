//
// Created by corentin on 20/04/15.
//

#ifndef STREAMS_LOG_H
#define STREAMS_LOG_H

#define LOG_EMERGENCY 0
#define LOG_ALERT 1
#define LOG_CRITICAL 2
#define LOG_ERROR 3
#define LOG_WARNING 4
#define LOG_NOTICE 5
#define LOG_INFO 6
#define LOG_DEBUG 7

#define LOG_FOLDER logs;
#define DISPLAY_LEVEL LOG_INFO

int log(char* message, int log_level);

#endif //STREAMS_LOG_H
