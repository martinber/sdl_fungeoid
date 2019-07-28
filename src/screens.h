#ifndef SCREENS_H
#define SCREENS_H

#include <stdbool.h>

#include "keyb.h"
#include "hud.h"
#include "input.h"
#include "field.h"

enum SCREEN_ID
{
    SCREEN_TITLE,
    SCREEN_GAME,
    SCREEN_FILE,
    SCREEN_CREDITS,
    SCREEN_TOTAL,
};

/// Game screen
typedef struct GameScreen
{
    Field *field;
    Keyboard *keyb;
    Hud *hud;
} GameScreen;

/// Contains the main loop of every screen.
/**
 * There are so few screens, so each one is handled differently depending on
 * what I need. For example the GameScreen stays in memory when the FileScreen
 * is opened.
 */
typedef struct ScreensHandler
{
    enum SCREEN_ID current_screen;
    InputHandler *input;
    SDL_Point window_size;
    bool loop_running; // If main loop is running

    GameScreen game_screen;
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
ScreensHandler *screens_init(SDL_Point window_size);

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
