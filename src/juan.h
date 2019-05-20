#ifndef JUAN_H
#define JUAN_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

/**
 * Helper functions for SDL.
 */

/// Start SDL and open the window.
/**
 * Returns 0 on success.
 */
int juan_init(
    SDL_Window **window,
    SDL_Renderer **renderer,
    const char *title,
    const int width,
    const int height
);

/// Load texture from PNG.
/**
 * Returns NULL if unsucessful.
 *
 * Caller should free the surface manually.
 */
SDL_Texture *juan_load_texture(SDL_Renderer *renderer, const char *path);

/// Load font from TTF.
/**
 * Returns NULL if unsucessful.
 *
 * Caller should free the font manually.
 */
TTF_Font *juan_load_font(const char *path, int size);

/// Draw text into a new texture.
/**
 * Returns NULL if unsucessful.
 *
 * Caller should free the texture manually.
 */
SDL_Texture *juan_text_texture(
    SDL_Renderer *renderer,
    TTF_Font *font,
    char *text,
    SDL_Color color
);

int juan_set_render_draw_color(SDL_Renderer *renderer, const SDL_Color *color);

#endif
