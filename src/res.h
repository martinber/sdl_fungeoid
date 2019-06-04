#ifndef RES_H
#define RES_H

#include "const.h"
#include "juan.h"

/**
 * Resources manager.
 *
 * TODO: Make this a struct just like the other modules
 *
 * Keeps track of textures, sounds, etc.
 */

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

/// Get texture by theme and id.
/*
 * Returns NULL on invalid IDs, e.g. INSTR_NULL or INSTR_SPACE.
 */
SDL_Texture *res_get_instr_tex(enum INSTR_THEME theme, enum INSTR_ID id);

#endif
