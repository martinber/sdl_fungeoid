#ifndef FIELD_H
#define FIELD_H

#include "juan.h"
#include "canvas.h"
#include "stack.h"
#include "input.h"
#include "keyb.h"
#include "res.h"
#include "drag.h"

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

    // Filename of current file, empty string if none. This filename will be
    // used if the "save" button is pressed
    char filename[256];

    // Absolute time in milliseconds when last simulation step was done
    Uint32 last_step_ms;
    // Time interval between steps
    Uint32 interval_step_ms;

    SDL_Point window_size;
    SDL_Point ip; // Instruction pointer
    SDL_Point speed; // Next relative motion of IP, e.g. (-1, 0)
    int cell_size;
    Stack *stack;

    DragState *canvas_drag;

} Field;

/// Create field
/**
 * Must be freed.
 *
 * On error returns NULL.
 */
Field *field_create(int width, int height, SDL_Point window_size);

/// User pressed "new"
/**
 * This function is called when the user presses the "new" button, but before
 * clearing we check if the user is sure.
 */
void field_new_intent(Field *field);

/// User pressed "load"
/**
 * This function is called when the user presses the "load" button, so a file
 * chooser will be opened
 */
void field_load_intent(Field *field);

/// User pressed "save"
/**
 * This function is called when the user presses the "save" button, but before
 * saving we check if a filename was already given, otherwise this function will
 * call field_save_as_intent().
 */
void field_save_intent(Field *field);

/// User pressed "save as"
/**
 * This function is called when the user presses the "save as" button, so a file
 * chooser will be opened.
 */
void field_save_as_intent(Field *field);

/// User selected file where to save
/**
 * This function is called when the user selected a filename on a "save as"
 * dialog.
 * Also remembers the filename given for subsequent field_save_intent()s.
 */
void field_save_filename_selected(Field *field, char *filename);

/// User selected file to load
/**
 * This function is called when the user selected a filename on a "load"
 * dialog.
 * Also remembers the filename given for subsequent field_save_intent()s.
 */
void field_load_filename_selected(Field *field, char *filename);

/// User pressed "new" and then pressed "I'm sure"
/**
 * This function is called when the user presses the "I'm sure" after being
 * asked if he really wanted to clear the canvas.
 */
void field_new_user_sure(Field *field);

/// Save canvas to autosave filename
/**
 * This function is called once in a while.
 * // TODO
 */
void field_autosave(Field *field);

void field_free(Field *field);

void field_update_geometry(Field *field, SDL_Point window_size);

/// Update interpreter if running
void field_update(Field *field, Uint32 time_abs_ms);

void field_handle_input(Field *field, Input *input);

void field_handle_keyb(Field *field, KeyboardEvent *event);

Stack *field_get_stack(Field *field);

void field_draw(SDL_Renderer *renderer, Field *field);

#endif
