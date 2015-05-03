/**
 * Corentin Badot-Bertrand cbadot-14
 * Corentin Dandoy cdandoy14
 *
 * Implémentation du serveur STREAMS.
 * Fonctions permettant de manipuler l'état de la partie en cours.
 * Par exemple, la phase dans laquelle le jeu se trouve.
 *
 * https://github.com/Saluki/Streams-Server
 */
#include "game.h"

int game_phase = REGISTER_PHASE;

int get_game_phase()
{
    return game_phase;
}

void set_game_phase(int phase)
{
    game_phase = phase;
}