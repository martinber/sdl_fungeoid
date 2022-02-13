#ifndef KEYB_H
#define KEYB_H

#include "juan.h"
#include "const.h"
#include "input.h"
#include "button.h"

// TODO remove and replace with a boolean?
enum KEYB_SHIFT_STATE
{
    KEYB_SHIFT_NONE,
    KEYB_SHIFT_1,
    KEYB_SHIFT_2,
    KEYB_SHIFT_STATE_TOTAL,
};

// Button IDs

enum KEYB_ARROW_BUTTON_ID
{
    KEYB_ARROW_UP,
    KEYB_ARROW_DOWN,
    KEYB_ARROW_LEFT,
    KEYB_ARROW_RIGHT,
    KEYB_ARROW_BUTTON_ID_TOTAL,
};

enum KEYB_ACTION_BUTTON_ID
{
    KEYB_ACTION_DELETE,
    KEYB_ACTION_SHIFT,
    KEYB_ACTION_SELECT_RECT,
    KEYB_ACTION_SELECT_PAINT,
    KEYB_ACTION_COPY,
    KEYB_ACTION_CUT,
    KEYB_ACTION_PASTE,
    KEYB_ACTION_COMMENT,
    KEYB_ACTION_BUTTON_ID_TOTAL,
};

enum KEYB_RUN_BUTTON_ID
{
    KEYB_RUN_START,
    KEYB_RUN_STOP,
    KEYB_RUN_STEP,
    KEYB_RUN_RESUME,
    KEYB_RUN_FASTER,
    KEYB_RUN_SLOWER,
    KEYB_RUN_BUTTON_ID_TOTAL,
};

enum KEYB_MISC_BUTTON_ID
{
    KEYB_MISC_NEW,
    KEYB_MISC_LOAD,
    KEYB_MISC_SAVE,
    KEYB_MISC_SAVE_AS,
    KEYB_MISC_T_HELP, // Toggle help
    KEYB_MISC_T_VALUES, // Toggle values
    KEYB_MISC_ZOOM_IN,
    KEYB_MISC_ZOOM_OUT,
    KEYB_MISC_QUIT,
    KEYB_MISC_BUTTON_ID_TOTAL,
};

enum KEYB_CHAR_BUTTON_ID
{
    KEYB_CHAR_GET,
    KEYB_CHAR_PUT,
    KEYB_CHAR_ADD_1,
    KEYB_CHAR_SUB_1,
    KEYB_CHAR_ADD_10,
    KEYB_CHAR_SUB_10,
    KEYB_CHAR_ADD_16,
    KEYB_CHAR_SUB_16,
    KEYB_CHAR_BUTTON_ID_TOTAL,
};

// IMPORTANT: Keep in sync with res.c:RES_KEYB_TAB_ID
enum KEYB_TAB_ID
{
    KEYB_TAB_RUN,
    KEYB_TAB_MISC,
    KEYB_TAB_VALUES,
    KEYB_TAB_MOVIO,
    KEYB_TAB_OPER,
    KEYB_TAB_CHAR,
    KEYB_TAB_ID_TOTAL,
};

/// On-screen keyboard
typedef struct Keyboard
{
    // Both rectangles determine the size of the keyboard for touch/click
    // purposes
    SDL_Rect geometry; // Excluding tabs
    SDL_Rect tabs_geometry; // Contains every tab

    enum KEYB_SHIFT_STATE shift_state;
    int button_size;

    int tabs_offset; // Offset in pixels used when dragging
    int tabs_min_offset;

    // Buttons for each tab
#define KEYB_VALUES_BUTTONS_TOTAL 16
#define KEYB_MOVIO_BUTTONS_TOTAL 16
#define KEYB_OPER_BUTTONS_TOTAL 14
    Button values_buttons[KEYB_VALUES_BUTTONS_TOTAL];
    Button movio_buttons[KEYB_MOVIO_BUTTONS_TOTAL];
    Button oper_buttons[KEYB_OPER_BUTTONS_TOTAL];
    Button run_buttons[KEYB_RUN_BUTTON_ID_TOTAL];
    Button misc_buttons[KEYB_MISC_BUTTON_ID_TOTAL];
    Button char_buttons[KEYB_CHAR_BUTTON_ID_TOTAL];

    // Remaining buttons available on most tabs
    Button arrow_buttons[KEYB_ARROW_BUTTON_ID_TOTAL];
    Button action_buttons[KEYB_ACTION_BUTTON_ID_TOTAL];

    // Tabs
    SDL_Rect tab_geometry[KEYB_TAB_ID_TOTAL];
    enum KEYB_TAB_ID active_tab;

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
    KEYB_EVENT_STEP,
    KEYB_EVENT_RESUME,
    KEYB_EVENT_FASTER,
    KEYB_EVENT_SLOWER,
    KEYB_EVENT_NEW,
    KEYB_EVENT_LOAD,
    KEYB_EVENT_SAVE,
    KEYB_EVENT_SAVE_AS,
    KEYB_EVENT_ZOOM_IN,
    KEYB_EVENT_ZOOM_OUT,
    KEYB_EVENT_QUIT,
    KEYB_EVENT_TYPE_TOTAL,
};

/// Keyboard event
/**
 * Used by keyb to send information to the input handler and then to the field.
 */
typedef struct KeyboardEvent
{
    enum KEYB_EVENT_TYPE type;
    enum INSTR_ID instr_id; // Used only if type is ADD_INSTR
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
