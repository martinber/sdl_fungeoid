#include "keyb.h"

#include "const.h"
#include "res.h"

Keyboard keyb_create(int window_width, int window_height, int button_size)
{
    SDL_Rect geometry = { 0, 0, 0, 0 };
    Keyboard keyb = { .geometry=geometry, .button_size=button_size };
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
    keyb->geometry.w = window_width;
    keyb->geometry.h = button_size * 5;
    keyb->geometry.x = 0;
    keyb->geometry.y = window_height - keyb->geometry.h;
}

void keyb_get_pos(Keyboard *keyb, int *x, int *y)
{
    *x = keyb->geometry.x;
    *y = keyb->geometry.y;
}

void keyb_get_size(Keyboard *keyb, int *width, int *height)
{
    *width = keyb->geometry.w;
    *height = keyb->geometry.h;
}

void keyb_draw(SDL_Renderer *renderer, Keyboard *keyb)
{
    {
        juan_set_render_draw_color(renderer, &COLOR_PANE_BG);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, &keyb->geometry);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }

    int margin = keyb->button_size / 2;
    int spacing = keyb->button_size / 8;
    int keyb_x = keyb->geometry.x;
    int keyb_y = keyb->geometry.y;
    int keyb_w = keyb->geometry.w;
    int keyb_h = keyb->geometry.h;

    // Draw arrows
    {
        int x = keyb_x + keyb_w - margin - keyb->button_size;
        int y = keyb_y + keyb_h - margin - keyb->button_size;
        SDL_Rect rect = { x, y, keyb->button_size, keyb->button_size };

        juan_set_render_draw_color(renderer, &COLOR_BUTTON_1);
        SDL_RenderFillRect(renderer, &rect);

        SDL_Texture* tex = res_get_instr_tex(INSTR_THEME_BEFUNGE_CHAR, INSTR_RIGHT);
        SDL_RenderCopy(renderer, tex, NULL, &rect);
    }
    {
        int x = keyb_x + keyb_w - margin - 2 * keyb->button_size - spacing;
        int y = keyb_y + keyb_h - margin - keyb->button_size;
        SDL_Rect rect = { x, y, keyb->button_size, keyb->button_size };

        juan_set_render_draw_color(renderer, &COLOR_BUTTON_1);
        SDL_RenderFillRect(renderer, &rect);

        SDL_Texture* tex = res_get_instr_tex(INSTR_THEME_BEFUNGE_CHAR, INSTR_DOWN);
        SDL_RenderCopy(renderer, tex, NULL, &rect);
    }
    {
        int x = keyb_x + keyb_w - margin - 3 * keyb->button_size - 2 * spacing;
        int y = keyb_y + keyb_h - margin - keyb->button_size;
        SDL_Rect rect = { x, y, keyb->button_size, keyb->button_size };

        juan_set_render_draw_color(renderer, &COLOR_BUTTON_1);
        SDL_RenderFillRect(renderer, &rect);

        SDL_Texture* tex = res_get_instr_tex(INSTR_THEME_BEFUNGE_CHAR, INSTR_LEFT);
        SDL_RenderCopy(renderer, tex, NULL, &rect);
    }
    {
        int x = keyb_x + keyb_w - margin - 2 * keyb->button_size - spacing;
        int y = keyb_y + keyb_h - margin - 2 * keyb->button_size - spacing;
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
