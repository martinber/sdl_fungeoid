#include "keyb.h"

#include "res.h"

Keyboard keyb_create(int window_width, int window_height, int button_size)
{
    // Initialize structs, leaving undefined geometries until I call
    // keyb_update_geometry()
    Keyboard keyb =
    {
        .geometry = { 0, 0, 0, 0 },
        .button_size = button_size,
        .shift_state = KEYB_SHIFT_NONE,
        // .instr_buttons = empty for now
        .but_up = { 0, 0, 0, 0 },
        .but_down = { 0, 0, 0, 0 },
        .but_left = { 0, 0, 0, 0 },
        .but_right = { 0, 0, 0, 0 },
        .but_shift_1 = { 0, 0, 0, 0 },
        .but_shift_2 = { 0, 0, 0, 0 },
    };

    // WARNING: Probably in the future I'll have to change the limits of this
    // loop
    for (enum INSTR_ID i = 0; i < INSTR_ID_TOTAL; i++)
    {
        InstrButton but =
        {
            .geometry = { 0, 0, 0, 0 },
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
    int keyb_x = keyb->geometry.x;
    int keyb_y = keyb->geometry.y;
    int keyb_w = keyb->geometry.w;
    int keyb_h = keyb->geometry.h;

    // Set buttons positions and sizes
    // WARNING: Probably in the future I'll have to change the limits of this
    // loop
    for (enum INSTR_ID i = 0; i < INSTR_ID_TOTAL; i++)
    {
        keyb->instr_buttons[i].geometry.w = button_size;
        keyb->instr_buttons[i].geometry.h = button_size;

        keyb->instr_buttons[i].geometry.x = keyb_x +
            margin + button_size * i + spacing * (i - 1);
        keyb->instr_buttons[i].geometry.y = keyb_y + margin;
    }

    keyb->but_up.x = keyb_x + keyb_w - margin - 2 * button_size - spacing;
    keyb->but_up.y = keyb_y + keyb_h - margin - 2 * button_size - spacing;
    keyb->but_up.w = button_size;
    keyb->but_up.h = button_size;

    keyb->but_down.x = keyb_x + keyb_w - margin - 2 * button_size - spacing;
    keyb->but_down.y = keyb_y + keyb_h - margin - button_size;
    keyb->but_down.w = button_size;
    keyb->but_down.h = button_size;

    keyb->but_left.x = keyb_x + keyb_w - margin - 3 * button_size - 2 * spacing;
    keyb->but_left.y = keyb_y + keyb_h - margin - button_size;
    keyb->but_left.w = button_size;
    keyb->but_left.h = button_size;

    keyb->but_right.x = keyb_x + keyb_w - margin - button_size;
    keyb->but_right.y = keyb_y + keyb_h - margin - button_size;
    keyb->but_right.w = button_size;
    keyb->but_right.h = button_size;

    keyb->but_shift_1.x = keyb_x + margin;
    keyb->but_shift_1.y = keyb_y + keyb_h - margin - 2 * button_size - spacing;
    keyb->but_shift_1.w = button_size;
    keyb->but_shift_1.h = button_size;

    keyb->but_shift_2.x = keyb_x + margin;
    keyb->but_shift_2.y = keyb_y + keyb_h - margin - button_size;
    keyb->but_shift_2.w = button_size;
    keyb->but_shift_2.h = button_size;
}

void keyb_draw(SDL_Renderer *renderer, Keyboard *keyb)
{
    {
        juan_set_render_draw_color(renderer, &COLOR_PANE_BG);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, &keyb->geometry);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }

    // Draw arrows

    SDL_Texture* tex = NULL;
    juan_set_render_draw_color(renderer, &COLOR_BUTTON_1);

    SDL_RenderFillRect(renderer, &keyb->but_up);
    tex = res_get_instr_tex(INSTR_THEME_BEFUNGE_CHAR, INSTR_UP);
    SDL_RenderCopy(renderer, tex, NULL, &keyb->but_up);

    SDL_RenderFillRect(renderer, &keyb->but_down);
    tex = res_get_instr_tex(INSTR_THEME_BEFUNGE_CHAR, INSTR_DOWN);
    SDL_RenderCopy(renderer, tex, NULL, &keyb->but_down);

    SDL_RenderFillRect(renderer, &keyb->but_left);
    tex = res_get_instr_tex(INSTR_THEME_BEFUNGE_CHAR, INSTR_LEFT);
    SDL_RenderCopy(renderer, tex, NULL, &keyb->but_left);

    SDL_RenderFillRect(renderer, &keyb->but_right);
    tex = res_get_instr_tex(INSTR_THEME_BEFUNGE_CHAR, INSTR_RIGHT);
    SDL_RenderCopy(renderer, tex, NULL, &keyb->but_right);

    // Draw shift
    juan_set_render_draw_color(renderer, &COLOR_BUTTON_2);
    SDL_RenderFillRect(renderer, &keyb->but_shift_1);
    juan_set_render_draw_color(renderer, &COLOR_BUTTON_3);
    SDL_RenderFillRect(renderer, &keyb->but_shift_2);

    // Draw buttons
    // WARNING: Probably in the future I'll have to change the limits of this
    // loop
    juan_set_render_draw_color(renderer, &COLOR_BUTTON_1);
    for (enum INSTR_ID i = 0; i < INSTR_ID_TOTAL; i++)
    {
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
        // Check arrows

        if (SDL_PointInRect(&input->point, &keyb->but_up))
        {
            event.type = KEYB_EVENT_MOVE_UP;
        }
        else if (SDL_PointInRect(&input->point, &keyb->but_down))
        {
            event.type = KEYB_EVENT_MOVE_DOWN;
        }
        else if (SDL_PointInRect(&input->point, &keyb->but_left))
        {
            event.type = KEYB_EVENT_MOVE_LEFT;
        }
        else if (SDL_PointInRect(&input->point, &keyb->but_right))
        {
            event.type = KEYB_EVENT_MOVE_RIGHT;
        }

        // Check instruction buttons
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
