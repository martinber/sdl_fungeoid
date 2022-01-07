#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include "keyb.h"
#include "hud.h"
#include "field.h"

/// Game screen
typedef struct Game
{
    Field *field;
    Keyboard *keyb;
    Hud *hud;

    SDL_Point _window_size;
} Game;

/// Create game screen
/**
 * Must be freed.
 *
 * On error returns NULL.
 */
Game *game_create(SDL_Point window_size);

void game_free(Game *game);

/// Called in every iteration of main loop
void game_update(Game *game, SDL_Renderer *renderer, Uint32 time_abs_ms);

/// Called when window is resized
void game_update_geometry(Game *game, SDL_Point window_size);

/// Handle Inputs
void game_handle_input(Game *game, Input *input);

/// Handle rest of SDL_Events that do not come in game_handle_input
void game_handle_event(Game *game, SDL_Event *event);

#endif
