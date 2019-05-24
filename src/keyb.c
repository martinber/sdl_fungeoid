#include "keyb.h"

#include "res.h"

Keyboard keyb_create(int window_width, int window_height, int button_size)
{
    // Initialize structs, leaving undefined geometries until I call
    // keyb_update_geometry()
    SDL_Rect geometry = { 0, 0, 0, 0 };
    Keyboard keyb =
    {
        .geometry = geometry,
        .button_size = button_size,
        .buttons_offset = 0,
        // .instr_buttons = empty for now
    };

    // WARNING: Probably in the future I'll have to change the limits of this
    // loop
    for (enum INSTR_ID i = 0; i < INSTR_ID_TOTAL; i++)
    {
        SDL_Rect but_geom = { 0, 0, 0, 0 };
        InstrButton but =
        {
            .geometry = but_geom,
            .id = i,
        };
        keyb.instr_buttons[i] = but;
    }

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
    // Set keyboard size
    keyb->button_size = button_size;
    keyb->geometry.w = window_width;
    keyb->geometry.h = button_size * 5;
    keyb->geometry.x = 0;
    keyb->geometry.y = window_height - keyb->geometry.h;

    int margin = keyb->button_size / 2;
    int spacing = keyb->button_size / 8;

    // Set buttons positions and sizes
    // WARNING: Probably in the future I'll have to change the limits of this
    // loop
    for (enum INSTR_ID i = 0; i < INSTR_ID_TOTAL; i++)
    {
        keyb->instr_buttons[i].geometry.w = button_size;
        keyb->instr_buttons[i].geometry.h = button_size;

        keyb->instr_buttons[i].geometry.x = keyb->geometry.x +
            margin + button_size * i + spacing * (i - 1);
        keyb->instr_buttons[i].geometry.y = keyb->geometry.y + margin;
    }
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

    // Draw buttons
    // WARNING: Probably in the future I'll have to change the limits of this
    // loop
    for (enum INSTR_ID i = 0; i < INSTR_ID_TOTAL; i++)
    {
        juan_set_render_draw_color(renderer, &COLOR_BUTTON_1);
        SDL_RenderFillRect(renderer, &keyb->instr_buttons[i].geometry);

        if (i != INSTR_NULL && i != INSTR_SPACE)
        {
            SDL_Texture* tex = res_get_instr_tex(INSTR_THEME_BEFUNGE_CHAR,
                keyb->instr_buttons[i].id);
            SDL_RenderCopy(renderer, tex, NULL, &keyb->instr_buttons[i].geometry);
        }
    }
}

KeyboardEvent keyb_handle_input
(
    Keyboard *keyb,
    Input *input
) {
    KeyboardEvent event =
    {
        .type = KEYB_EVENT_NONE,
        .instr_id = INSTR_NULL,
    };

    if (input->type == INPUT_CLICK_UP)
    {
        // WARNING: Probably in the future I'll have to change the limits of this
        // loop
        for (enum INSTR_ID i = 0; i < INSTR_ID_TOTAL; i++)
        {
            if (SDL_PointInRect(&input->point, &keyb->instr_buttons[i].geometry))
            {
                if (
                    keyb->instr_buttons[i].id != INSTR_NULL
                    && keyb->instr_buttons[i].id != INSTR_SPACE
                ) {
                    event.type = KEYB_EVENT_ADD_INSTR;
                    event.instr_id = keyb->instr_buttons[i].id;
                }
                else
                {
                    event.type = KEYB_EVENT_RM_INSTR;
                    event.instr_id = keyb->instr_buttons[i].id;
                }
                break;
            }
        }
    }
    else if (input->type == INPUT_CLICK_MOVE)
    {
        // WARNING: Probably in the future I'll have to change the limits of this
        // loop
        for (enum INSTR_ID i = 0; i < INSTR_ID_TOTAL; i++)
        {
            keyb->instr_buttons[i].geometry.x += input->diff.x;
        }
    }
    return event;
}
