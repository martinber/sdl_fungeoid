#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include "intrpr.h"
#include "keyb.h"
#include "hud.h"
#include "field.h"

/// Game screen
typedef struct Game
{
    Intrpr *_intrpr;
    Field *_field;
    Keyboard *_keyb;
    Hud *_hud;

    // Filename of current file, empty string if none. This filename will be
    // used if the "save" button is pressed
    char _filename[256];

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

/// User pressed "new"
/**
 * This function is called when the user presses the "new" button, but before
 * clearing we check if the user is sure.
 */
void game_new_intent(Game *game);

/// User pressed "load"
/**
 * This function is called when the user presses the "load" button, so a file
 * chooser will be opened
 */
void game_load_intent(Game *game);

/// User pressed "save"
/**
 * This function is called when the user presses the "save" button, but before
 * saving we check if a filename was already given, otherwise this function will
 * call game_save_as_intent().
 */
void game_save_intent(Game *game);

/// User pressed "save as"
/**
 * This function is called when the user presses the "save as" button, so a file
 * chooser will be opened.
 */
void game_save_as_intent(Game *game);

/// User selected file where to save
/**
 * This function is called when the user selected a filename on a "save as"
 * dialog.
 * Also remembers the filename given for subsequent game_save_intent()s.
 */
void game_save_filename_selected(Game *game, char *filename);

/// User selected file to load
/**
 * This function is called when the user selected a filename on a "load"
 * dialog.
 * Also remembers the filename given for subsequent game_save_intent()s.
 */
void game_load_filename_selected(Game *game, char *filename);

/// User pressed "new" and then pressed "I'm sure"
/**
 * This function is called when the user presses the "I'm sure" after being
 * asked if he really wanted to clear the canvas.
 */
void game_new_user_sure(Game *game);

/// Save canvas to autosave filename
/**
 * This function is called once in a while.
 * // TODO
 */
void game_autosave(Game *game);


#endif
