#ifndef JUAN_H
#define JUAN_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
/// Can't include anything else to avoid circular dependencies!

/**
 * Helper functions for SDL.
 */

/// Start SDL and open the window.
/**
 * Returns 0 on success.
 *
 * On GNU/Linux tries to open a resizable window with the given size, on Android
 * the size will be ignored and the screen size will be used instead.
 */
int juan_init(
    SDL_Window **window,
    SDL_Renderer **renderer,
    const char *title,
    int width,
    int height
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

float juan_point_abs(SDL_Point *a);

/// Absolute value squared
float juan_point_sq_abs(SDL_Point *a);

SDL_Point juan_point_add(SDL_Point *a, SDL_Point *b);

SDL_Point juan_point_sub(SDL_Point *a, SDL_Point *b);

/// Distance squared
float juan_point_sq_dist(SDL_Point *a, SDL_Point *b);

/// Convert float to int and check for overflow
int juan_ftoi(float x);

/// Draw horizontal line with square cap.
/**
 * Width should be even.
 *
 * The line has a square cap on both ends, so the line is actually width pixels
 * longer than the given length.
 */
void juan_draw_h_line_cap
(
    SDL_Renderer *renderer,
    int x,
    int y,
    int length,
    int width
);

/// Draw vertical line with square cap.
/**
 * Width should be even.
 *
 * The line has a square cap on both ends, so the line is actually width pixels
 * longer than the given length.
 */
void juan_draw_v_line_cap
(
    SDL_Renderer *renderer,
    int x,
    int y,
    int length,
    int width
);

#endif
