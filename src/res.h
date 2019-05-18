#ifndef RES_H
#define RES_H

#include "juan.h"

enum
{
    RES_TEX_BALL,
    RES_TEX_CHAR_G,
    RES_TEX_TOTAL
};

SDL_Texture *RES_TEXTURES[RES_TEX_TOTAL];

/**
 * Resources manager.
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
 *
 */
void res_free_all();

#endif
