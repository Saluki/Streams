/**
 * Corentin Badot-Bertrand cbadot-14
 * Corentin Dandoy cdandoy14
 *
 * Implémentation du serveur STREAMS.
 * Fichier contenant les définitions des fonctions de log.c ainsi que de constantes pour pouvoir
 * utiliser les fonctions de log.
 *
 * https://github.com/Saluki/Streams-Server
 */
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

#define LOG_FOLDER "streams_logs"

int log_message(char* message, int log_level);
int log_error(char* message, int log_level, int errno_number);
void set_log_method(char** argv);

#endif //STREAMS_LOG_H
