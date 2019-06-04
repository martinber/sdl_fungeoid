#ifndef HUD_H
#define HUD_H

#include "juan.h"
#include "input.h"
#include "stack.h"

enum HUD_STATE
{
    HUD_STATE_OPEN,
    HUD_STATE_CLOSED,
    HUD_STATE_TOTAL,
};

typedef struct Hud
{
    SDL_Rect geometry;
    SDL_Rect tab_geometry;
    SDL_Point window_size;
    int line_spacing;
    enum HUD_STATE state;
    Stack *stack;
} Hud;

/// Create hud
/**
 * Must be freed.
 *
 * On error returns NULL.
 */
Hud *hud_create(SDL_Point window_size, int line_height, Stack *stack);

void hud_free(Hud *hud);

/// Update size and position from window and line height
void hud_update_geometry(Hud *hud, SDL_Point window_size, int line_height);

/// Draw keyboard
void hud_draw(SDL_Renderer *renderer, Hud *hud);

/// Handle touch/mouse/key input
/**
 * Returns 1 if the input lies outside of the hud and should be handled by
 * another element
 */
int hud_handle_input(Hud *hud, Input *input);

#endif
