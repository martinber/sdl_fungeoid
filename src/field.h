#ifndef FIELD_H
#define FIELD_H

#include "juan.h"
#include "canvas.h"
#include "input.h"
#include "keyb.h"
#include "res.h"

/**
 * Contains a canvas with instructions, keeps track of program execution, etc.
 */

typedef struct Field
{
    Canvas canvas;
    SDL_Point screen_size;
    SDL_Point ip; // Instruction pointer
    SDL_Point speed; // Next relative motion of IP, e.g. (-1, 0)
    int cell_size;
} Field;

/// Create field
/**
 * Must be freed.
 *
 * On error TODO
 */
Field field_create(int width, int height, SDL_Point *screen_size, int cell_size);

void field_resize_screen(Field *field, SDL_Point *screen_size, int cell_size);

int field_free(Field *field);

void field_handle_input(Field *field, Input *input);

void field_handle_keyb(Field *field, KeyboardEvent *event);

void field_draw(SDL_Renderer *renderer, Field *field);

#endif
