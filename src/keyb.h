#ifndef KEYB_H
#define KEYB_H

#include "juan.h"
#include "const.h"
#include "input.h"

/**
 * On-screen keyboard
 */

typedef struct InstrButton
{
    SDL_Rect geometry;
    enum INSTR_ID id;
} InstrButton;

enum KEYB_SHIFT_STATE
{
    KEYB_SHIFT_NONE,
    KEYB_SHIFT_1,
    KEYB_SHIFT_2,
    KEYB_SHIFT_STATE_TOTAL,
};

// IMPORTANT: Keep in sync with res.c:RES_KEYB_TAB_ID
enum KEYB_TAB_ID
{
    KEYB_TAB_RUN,
    KEYB_TAB_VALUES,
    KEYB_TAB_MOVIO,
    KEYB_TAB_OPER,
    KEYB_TAB_CHAR,
    KEYB_TAB_ID_TOTAL,
};

typedef struct Keyboard
{
    // Both rectangles determine the size of the keyboard for touch/click
    // purposes
    SDL_Rect geometry; // Excluding tabs
    SDL_Rect tabs_geometry; // Contains every tab

    enum KEYB_SHIFT_STATE shift_state;

    // Buttons for each tab
#define KEYB_VALUES_BUTTONS_TOTAL 16
    InstrButton values_buttons[KEYB_VALUES_BUTTONS_TOTAL];
#define KEYB_MOVIO_BUTTONS_TOTAL 16
    InstrButton movio_buttons[KEYB_MOVIO_BUTTONS_TOTAL];
#define KEYB_OPER_BUTTONS_TOTAL 14
    InstrButton oper_buttons[KEYB_OPER_BUTTONS_TOTAL];

    // Tabs
    SDL_Rect tab_geometry[KEYB_TAB_ID_TOTAL];
    enum KEYB_TAB_ID active_tab;

    // Remaining buttons
    SDL_Rect but_up;
    SDL_Rect but_down;
    SDL_Rect but_left;
    SDL_Rect but_right;
    SDL_Rect but_shift_1;
    SDL_Rect but_shift_2;
    SDL_Rect but_select_rect;
    SDL_Rect but_select_paint;
    SDL_Rect but_copy;
    SDL_Rect but_cut;
    SDL_Rect but_paste;
    SDL_Rect but_comment;
} Keyboard;

enum KEYB_EVENT_TYPE
{
    KEYB_EVENT_NONE,
    KEYB_EVENT_NOT_HANDLED,
    KEYB_EVENT_ADD_INSTR,
    KEYB_EVENT_RM_INSTR,
    KEYB_EVENT_MOVE_UP,
    KEYB_EVENT_MOVE_DOWN,
    KEYB_EVENT_MOVE_LEFT,
    KEYB_EVENT_MOVE_RIGHT,
    KEYB_EVENT_START,
    KEYB_EVENT_STOP,
    KEYB_EVENT_TYPE_TOTAL,
};

typedef struct KeyboardEvent
{
    enum KEYB_EVENT_TYPE type;
    enum INSTR_ID instr_id;
} KeyboardEvent;

/// Create keyb
/**
 * Must be freed.
 *
 * On error returns NULL.
 */
Keyboard *keyb_create(SDL_Point window_size);

void keyb_free(Keyboard *keyb);

/// Update size and position from window and cell size
void keyb_update_geometry(Keyboard *keyb, SDL_Point window_size);

/// Draw keyboard
void keyb_draw(SDL_Renderer *renderer, Keyboard *keyb);

/// Handle touch/mouse/key input
/**
 * Returns event of type KEY_EVENT_NOT_HANDLED if the input lies outside of the
 * keyboard and should be handled by another element
 */
KeyboardEvent keyb_handle_input(Keyboard *keyb, Input *input);

#endif
