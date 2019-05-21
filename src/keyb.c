#include "keyb.h"

#include "const.h"
#include "res.h"

Keyboard keyb_create(int window_width, int window_height, int button_size)
{
    Keyboard keyb = { 0, 0, 0, 0, 0 };
    keyb_update_geometry(&keyb, window_width, window_height, button_size);
    return keyb;
}

void keyb_update_geometry
(
    Keyboard *keyb,
    int window_width,
    int window_height,
    int button_size
) {
    keyb->button_size = button_size;
    keyb->width = window_width;
    keyb->height = button_size * 5;
    keyb->x = 0;
    keyb->y = window_height - keyb->height;
}

void keyb_get_pos(Keyboard *keyb, int *x, int *y)
{
    *x = keyb->x;
    *y = keyb->y;
}

void keyb_get_size(Keyboard *keyb, int *width, int *height)
{
    *width = keyb->width;
    *height = keyb->height;
}

void keyb_draw(SDL_Renderer *renderer, Keyboard *keyb)
{
    {
        juan_set_render_draw_color(renderer, &COLOR_PANE_BG);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_Rect rect = { keyb->x, keyb->y, keyb->width, keyb->height };
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }

    int margin = keyb->button_size / 2;
    int spacing = keyb->button_size / 8;

    // Draw arrows
    {
        int x = keyb->x + keyb->width - margin - keyb->button_size;
        int y = keyb->y + keyb->height - margin - keyb->button_size;
        SDL_Rect rect = { x, y, keyb->button_size, keyb->button_size };

        juan_set_render_draw_color(renderer, &COLOR_BUTTON_1);
        SDL_RenderFillRect(renderer, &rect);

        SDL_Texture* tex = res_get_instr_tex(INSTR_THEME_BEFUNGE_CHAR, INSTR_RIGHT);
        SDL_RenderCopy(renderer, tex, NULL, &rect);
    }
    {
        int x = keyb->x + keyb->width - margin - 2 * keyb->button_size - spacing;
        int y = keyb->y + keyb->height - margin - keyb->button_size;
        SDL_Rect rect = { x, y, keyb->button_size, keyb->button_size };

        juan_set_render_draw_color(renderer, &COLOR_BUTTON_1);
        SDL_RenderFillRect(renderer, &rect);

        SDL_Texture* tex = res_get_instr_tex(INSTR_THEME_BEFUNGE_CHAR, INSTR_DOWN);
        SDL_RenderCopy(renderer, tex, NULL, &rect);
    }
    {
        int x = keyb->x + keyb->width - margin - 3 * keyb->button_size - 2 * spacing;
        int y = keyb->y + keyb->height - margin - keyb->button_size;
        SDL_Rect rect = { x, y, keyb->button_size, keyb->button_size };

        juan_set_render_draw_color(renderer, &COLOR_BUTTON_1);
        SDL_RenderFillRect(renderer, &rect);

        SDL_Texture* tex = res_get_instr_tex(INSTR_THEME_BEFUNGE_CHAR, INSTR_LEFT);
        SDL_RenderCopy(renderer, tex, NULL, &rect);
    }
    {
        int x = keyb->x + keyb->width - margin - 2 * keyb->button_size - spacing;
        int y = keyb->y + keyb->height - margin - 2 * keyb->button_size - spacing;
        SDL_Rect rect = { x, y, keyb->button_size, keyb->button_size };

        juan_set_render_draw_color(renderer, &COLOR_BUTTON_1);
        SDL_RenderFillRect(renderer, &rect);

        SDL_Texture* tex = res_get_instr_tex(INSTR_THEME_BEFUNGE_CHAR, INSTR_UP);
        SDL_RenderCopy(renderer, tex, NULL, &rect);
    }
}

/// Handle fingerdown event
void keyb_handle_fingerdown
(
    Keyboard *keyb,
    int x,
    int y
) { }

/// Handle fingermotion event
void keyb_handle_fingermotion
(
    Keyboard *keyb,
    int x,
    int y
) { }

/// Handle fingerup event
void keyb_handle_fingerup
(
    Keyboard *keyb,
    int x,
    int y
) { }
