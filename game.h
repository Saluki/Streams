/**
 * Corentin Badot-Bertrand cbadot-14
 * Corentin Dandoy cdandoy14
 *
 * Serveur STREAMS
 * Fichier header de game.c
 */
#ifndef STREAMS_GAME_H
#define STREAMS_GAME_H

#define REGISTER_PHASE 0
#define GAME_PHASE 1

#define WAIT_TIME 30

int get_game_phase();
void set_game_phase(int phase);

#endif //STREAMS_GAME_H
