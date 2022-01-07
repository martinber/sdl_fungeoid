#ifndef SCREENS_H
#define SCREENS_H

#include <stdbool.h>

#include "game.h"
#include "input.h"

enum SCREEN_ID
{
    SCREEN_TITLE,
    SCREEN_GAME,
    SCREEN_FILE,
    SCREEN_CREDITS,
    SCREEN_TOTAL,
};

/// Contains the main loop of every screen.
/**
 * There are so few screens, so each one is handled differently depending on
 * what I need.
 */
typedef struct ScreensHandler
{
    SDL_Point window_size;

    enum SCREEN_ID _current_screen;
    InputHandler *_input;
    bool _loop_running; // If main loop is running

    // Screen of game, so far the only screen
    Game *_game;
} ScreensHandler;


/// Initialize everything.
/**
 * Initialize every screen when the app is opened, if needed further things will
 * be initialized when needed transparently.
 *
 * On error returns NULL.
 *
 * Must be freed with screens_free().
 */
ScreensHandler *screens_create(SDL_Point window_size);

/// Free everything.
/**
 * Delete every screen.
 */
void screens_free(ScreensHandler *screens);


/// Start the main loop
/**
 * Called by main.
 */
void screens_loop(ScreensHandler *screens, SDL_Renderer *renderer);

#endif
