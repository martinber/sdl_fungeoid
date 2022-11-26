#ifndef TEXT_H
#define TEXT_H

#include <stdbool.h>

#include "juan.h"

enum TEXT_ALIGN
{
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_START, // Means left or top depending if we talk of vertical or horizontal align
    TEXT_ALIGN_END, // Means right or bottom depending if we talk of vertical or horizontal align
};

/// Keeps track of geometry and texture of a text, updates the texture only if necessary
typedef struct Text
{
    SDL_Point position;
    SDL_Color color;
    TTF_Font *font;
    enum TEXT_ALIGN h_align;
    enum TEXT_ALIGN v_align;

    int _size; // Max amount of chars
    // TODO: Allow to set line height
    char *_str; // Last string set by the user

    // Texture and flag indicating if the _str changed, this way the texture is updated before next
    // draw
    SDL_Texture *_texture;
    bool _update_texture;

} Text;

/// Create text
/**
 * Must be freed.
 *
 * On error returns NULL.
 */
Text *text_create(int size, TTF_Font *font, SDL_Color color);

void text_free(Text *text);

void text_set_str(Text *text, char *str);

void text_draw(SDL_Renderer *renderer, Text *text);

#endif
