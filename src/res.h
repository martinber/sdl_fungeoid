#ifndef RES_H
#define RES_H

#include "const.h"
#include "juan.h"

/// Resources management
/**
 * TODO: Make this a struct just like the other modules
 *
 * Keeps track of textures, sounds, etc.
 */

enum RES_FONT_ID
{
    RES_FONT_STACK,
    RES_FONT_ID_TOTAL,
};

enum RES_KEYB_ICON_ID
{
    RES_KEYB_ICON_DELETE,
    RES_KEYB_ICON_SHIFT,
    RES_KEYB_ICON_SELECT_RECT,
    RES_KEYB_ICON_SELECT_PAINT,
    RES_KEYB_ICON_COPY,
    RES_KEYB_ICON_CUT,
    RES_KEYB_ICON_PASTE,
    RES_KEYB_ICON_COMMENT,
    RES_KEYB_ICON_START,
    RES_KEYB_ICON_STOP,
    RES_KEYB_ICON_STEP,
    RES_KEYB_ICON_RESUME,
    RES_KEYB_ICON_TIME_FASTER,
    RES_KEYB_ICON_TIME_SLOWER,
    RES_KEYB_ICON_NEW,
    RES_KEYB_ICON_LOAD,
    RES_KEYB_ICON_SAVE,
    RES_KEYB_ICON_SAVE_AS,
    RES_KEYB_ICON_T_HELP,
    RES_KEYB_ICON_T_VALUES,
    RES_KEYB_ICON_QUIT,
    RES_KEYB_ICON_ZOOM_IN,
    RES_KEYB_ICON_ZOOM_OUT,
    RES_KEYB_ICON_CHAR_GET,
    RES_KEYB_ICON_CHAR_PUT,
    RES_KEYB_ICON_CHAR_ADD_1,
    RES_KEYB_ICON_CHAR_SUB_1,
    RES_KEYB_ICON_CHAR_ADD_10,
    RES_KEYB_ICON_CHAR_SUB_10,
    RES_KEYB_ICON_CHAR_ADD_16,
    RES_KEYB_ICON_CHAR_SUB_16,
    RES_KEYB_ICON_ARROW_UP,
    RES_KEYB_ICON_ID_TOTAL,
};

// IMPORTANT: Keep in sync with keyb.c:KEYB_TAB_ID
enum RES_KEYB_TAB_ID
{
    RES_KEYB_TAB_RUN,
    RES_KEYB_TAB_MISC,
    RES_KEYB_TAB_VALUES,
    RES_KEYB_TAB_MOVIO,
    RES_KEYB_TAB_OPER,
    RES_KEYB_TAB_CHAR,
    RES_KEYB_TAB_ID_TOTAL,
};

/// Load all resources.
/**
 * Returns 0 on success.
 *
 * If some texture fails to load, deletes any texture loaded. If the textures
 * are loaded, the caller must use res_free_all().
 */
int res_load_all(SDL_Renderer *renderer);

/// Free all resources.
/**
 * Can free resources correctly even when res_load_all() failed (that is, when
 * there are textures pointing to NULL).
 */
void res_free_all();

/// Get instruction texture by theme and id.
/*
 * Returns NULL on invalid IDs, e.g. INSTR_NULL or INSTR_SPACE.
 */
SDL_Texture *res_get_instr_tex(enum INSTR_THEME theme, enum INSTR_ID id);

/// Get instruction texture by theme and char.
SDL_Texture *res_get_instr_char_tex(enum INSTR_THEME theme, char c);

SDL_Texture *res_get_keyb_icon_tex(enum INSTR_THEME theme, enum RES_KEYB_ICON_ID id);

SDL_Texture *res_get_keyb_tab_tex(enum INSTR_THEME theme, enum RES_KEYB_TAB_ID id);

TTF_Font *res_get_font(enum RES_FONT_ID font);

#endif
