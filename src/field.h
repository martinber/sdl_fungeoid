#ifndef FIELD_H
#define FIELD_H

#include "juan.h"
#include "intrpr.h"
#include "input.h"
#include "keyb.h"
#include "res.h"
#include "drag.h"

/// Draws the field/canvas and handles user input to control the intrpr and edit
/// code.
typedef struct Field
{
    SDL_Point _window_size;
    int _cell_size;

    DragState *_drag;
    Intrpr *_intrpr; // A reference, owned by Game
} Field;

/// Create field
/**
 * Must be freed.
 *
 * On error returns NULL.
 */
Field *field_create(SDL_Point window_size, Intrpr *intrpr);

void field_free(Field *field);

void field_update_geometry(Field *field, SDL_Point window_size);

/// Update and draw
void field_update(Field *field, Uint32 time_abs_ms);

void field_handle_input(Field *field, Input *input);

void field_handle_keyb(Field *field, KeyboardEvent *event);

Stack *field_get_stack(Field *field);

void field_draw(SDL_Renderer *renderer, Field *field);

#endif
