#ifndef FIELD_H
#define FIELD_H

#include "juan.h"
#include "canvas.h"
#include "stack.h"
#include "input.h"
#include "keyb.h"
#include "res.h"

enum FIELD_STATE
{
    FIELD_EDITING,
    FIELD_RUNNING,
    FIELD_STATE_TOTAL,
};

/// This is the main game struct
/**
 * Contains a canvas with instructions, keeps track of program execution, etc.
 */
typedef struct Field
{
    Canvas *canvas;
    enum FIELD_STATE state;

    // Absolute time in milliseconds when last simulation step was done
    Uint32 last_step_ms;

    SDL_Point screen_size;
    SDL_Point ip; // Instruction pointer
    SDL_Point speed; // Next relative motion of IP, e.g. (-1, 0)
    int cell_size;
    Stack *stack;
} Field;

/// Create field
/**
 * Must be freed.
 *
 * On error returns NULL.
 */
Field *field_create(int width, int height, SDL_Point *screen_size, int cell_size);

/// Load a program from a file
/**
 * The filename should be a string ready to be used by fopen()
 */
void field_load_file(Field *field, char *filename);

void field_free(Field *field);

void field_resize_screen(Field *field, SDL_Point *screen_size, int cell_size);

/// Update interpreter if running
void field_update(Field *field, Uint32 time_abs_ms);

void field_handle_input(Field *field, Input *input);

void field_handle_keyb(Field *field, KeyboardEvent *event);

Stack *field_get_stack(Field *field);

void field_draw(SDL_Renderer *renderer, Field *field);

#endif
