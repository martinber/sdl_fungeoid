#ifndef JUAN_H
#define JUAN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

/// Load surface from PNG
/**
 * Returns NULL if unsucessful.
 */
SDL_Texture* juan_load_texture(SDL_Renderer *renderer, const char *path);

#endif
