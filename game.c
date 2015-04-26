//
// Created by corentin on 25/04/15.
//

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