/**
 * Corentin Badot-Bertrand cbadot-14
 * Corentin Dandoy cdandoy14
 *
 * Implémentation du serveur STREAMS.
 * Fichier contenant les définitions des fonctions de game.c ainsi que certaines
 * constantes liées au jeu: les temps d'attente, le nombre maximal d'utilisateurs,...
 *
 * https://github.com/Saluki/Streams-Server
 */
#ifndef STREAMS_GAME_H
#define STREAMS_GAME_H

#define REGISTER_PHASE 0
#define GAME_PHASE 1

#define WAIT_TIME 30
#define MAX_NUMBER_USERS 10

int get_game_phase();
void set_game_phase(int phase);

#endif //STREAMS_GAME_H
