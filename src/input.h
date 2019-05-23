#ifndef INPUT_H
#define INPUT_H

#include "juan.h"

enum INPUT_TYPE
{
    INPUT_NONE,
    INPUT_CLICK_MOVE,
    INPUT_CLICK_MOVE_UP,
    INPUT_CLICK_UP,
    INPUT_KEY_DOWN,
    INPUT_KEY_UP,
    INPUT_TYPE_TOTAL,
};

/// Keeps state of input
/**
 * Consider the members as private.
 *
 * For example remembers if the mouse was pressed to detect dragging. Only
 * detect first finger and left click, ignore the rest of fingers and buttons.
 *
 * Supported touch/mouse interactions:
 *
 * - Touch down and up: Returns only INPUT_CLICK_UP.
 *
 * - Touch down, drag and up: Returns nothing on touch down, the, if the
 *   distance moved is larger than a threshold returs INPUT_CLICK_MOVE. Finally
 *   returns INPUT_CLICK_MOVE_UP on release.
 */
typedef struct InputHandler
{
    // Most of the time is INPUT_NONE, when a touch down or mouse down
    // event is received, the candidate is INPUT_CLICK_UP, but when there was a
    // considerable amount of dragging the candidate is changed to
    // INPUT_CLICK_MOVE_UP until the finger or mouse button is released
    enum INPUT_TYPE click_candidate;

    // Last finger ID, set to zero if using the mouse. Could give problems if
    // someone uses the mouse and fingers at the same time but what gives.
    SDL_FingerID last_touch_id;

    // Where the mouse or finger went down
    SDL_Point down_point;

    // Move distance threshold, in pixels squared
    int sq_threshold;
} InputHandler;

typedef struct Input
{
    enum INPUT_TYPE type;

    // for INPUT_CLICK_UP, INPUT_CLICK_MOVE and INPUT_MOVE_UP
    SDL_Point point;

    // for INPUT_CLICK_MOVE and INPUT_MOVE_UP
    SDL_Point down_point;
    SDL_Point dist;

    // for INPUT_CLICK_MOVE
    SDL_Point diff;

    // for INPUT_MOVE_UP
    // SDL_Point speed;

    // for INPUT_KEY_DOWN and INPUT_KEY_UP
    SDL_Keycode key;
} Input;

/// Create InputHandler
InputHandler input_create();

/// Handle input events
/**
 * Touch events, mouse events and keyboard events.
 */
Input input_handle_event(InputHandler *handler, SDL_Point *window_size, SDL_Event *event);

#endif
