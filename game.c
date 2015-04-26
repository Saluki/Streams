/**
 * Corentin Badot-Bertrand cbadot-14
 * Corentin Dandoy cdandoy14
 *
 * Serveur STREAMS
 * Code permettant de manipuler l'etat de la partie en cours
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