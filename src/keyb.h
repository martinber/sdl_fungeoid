#ifndef KEYB_H
#define KEYB_H

#include "juan.h"
#include "const.h"

/**
 * On-screen keyboard
 */

typedef struct InstrButton
{
    SDL_Rect geometry;
    enum INSTR_ID id;
} InstrButton;

typedef struct Keyboard
{
    SDL_Rect geometry;
    int button_size;

    // One button per instruction ignoring the fact that INSTR_NULL wont be
    // used
    InstrButton instr_buttons[INSTR_ID_TOTAL];
} Keyboard;

enum KEYB_EVENT_TYPE
{
    KEYB_EVENT_NONE,
    KEYB_EVENT_ADD_INSTR,
    KEYB_EVENT_RM_INSTR,
    KEYB_EVENT_TYPE_TOTAL,
};

typedef struct KeyboardEvent
{
    enum KEYB_EVENT_TYPE type;
    enum INSTR_ID instr_id;
} KeyboardEvent;

/// Create keyb
Keyboard keyb_create(int window_width, int window_height, int button_size);

/// Update size and position from window and cell size
void keyb_update_geometry
(
    Keyboard *keyb,
    int window_width,
    int window_height,
    int button_size
);

/// Draw keyboard
void keyb_draw(SDL_Renderer *renderer, Keyboard *keyb);

/// Handle fingerdown event
KeyboardEvent keyb_handle_fingerdown
(
    Keyboard *keyb,
    SDL_Point *point
);

/// Handle fingermotion event
KeyboardEvent keyb_handle_fingermotion
(
    Keyboard *keyb,
    SDL_Point *point
);

/// Handle fingerup event
KeyboardEvent keyb_handle_fingerup
(
    Keyboard *keyb,
    SDL_Point *point
);

#endif
