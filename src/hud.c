#include "hud.h"

#include "res.h"

static void hud_toggle(Hud *hud)
{
    switch (hud->state)
    {
        case HUD_STATE_OPEN:
            hud->state = HUD_STATE_CLOSED;
            break;
        case HUD_STATE_CLOSED:
            hud->state = HUD_STATE_OPEN;
            break;
        default:
            break;
    }
    hud_update_geometry(hud, hud->window_size, hud->line_spacing);
}

Hud *hud_create(SDL_Point window_size, int line_height)
{
    Hud *hud = (Hud*) malloc(sizeof(Hud));
    if (hud == NULL)
    {
        SDL_Log("Failed to malloc Hud");
        return NULL;
    }
    hud->geometry = (SDL_Rect) { 0, 0, 0, 0 };
    hud->tab_geometry = (SDL_Rect) { 0, 0, 0, 0 };
    hud->window_size = window_size;
    hud->line_spacing = line_height;
    hud->state = HUD_STATE_OPEN;

    hud_update_geometry(hud, window_size, line_height);
    return hud;
}

void hud_free(Hud *hud)
{
    free(hud);
    hud = NULL;
}

void hud_update_geometry(Hud *hud, SDL_Point window_size, int line_height)
{
    hud->window_size = window_size;

    hud->geometry.w = window_size.x / 4;
    hud->geometry.h = window_size.y * 2 / 3;
    hud->geometry.y = 0;
    hud->tab_geometry.w = 32;
    hud->tab_geometry.h = 64;
    hud->tab_geometry.y = 64;
    switch (hud->state)
    {
        case HUD_STATE_OPEN:
            hud->geometry.x = window_size.x - hud->geometry.w;
            break;

        case HUD_STATE_CLOSED:
            hud->geometry.x = window_size.x;
            break;

        default:
            break;
    }
    hud->tab_geometry.x = hud->geometry.x - hud->tab_geometry.w;

    hud->line_spacing = line_height;
}

void hud_draw(SDL_Renderer *renderer, Hud *hud)
{
    {
        juan_set_render_draw_color(renderer, &COLOR_PANE_BG);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, &hud->geometry);
        SDL_RenderFillRect(renderer, &hud->tab_geometry);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }

    // TODO: improve
    int STACK_SIZE = 7;
    int stack[7] = { 1, 45, 73, 24, 63, 7345, 65475676 };
    SDL_Texture *tex = NULL;
    SDL_Rect position = { 0, 0, 0, 0 };
    char str_buf[10]; // Buffer of maximum 9 chars
    for (int i = 0; i < STACK_SIZE; i++)
    {
        snprintf(str_buf, 9, "%d", stack[i]);
        str_buf[8] = '~';
        str_buf[9] = '\0';
        tex = juan_text_texture
        (
            renderer,
            res_get_font(RES_FONT_STACK),
            str_buf,
            COLOR_WHITE
        );
        if (tex == NULL)
        {
            SDL_Log("Error rendering text for stack value: %d", stack[i]);
            return;
        }
        SDL_QueryTexture(tex, NULL, NULL, &position.w, &position.h);
        position.x = hud->geometry.x + 32;
        position.y = hud->geometry.y + 32 + hud->line_spacing * i;
        SDL_RenderCopy(renderer, tex, NULL, &position);
        SDL_DestroyTexture(tex);
    }
}

int hud_handle_input(Hud *hud, Input *input)
{
    switch (input->type)
    {
        case (INPUT_CLICK_UP):
            if (
                !SDL_PointInRect(&input->point, &hud->geometry)
                && !SDL_PointInRect(&input->point, &hud->tab_geometry)
            ) {
                return 1; // Not handled
            }
            break;
        case (INPUT_CLICK_MOVE):
        case (INPUT_CLICK_MOVE_UP):
            if (
                !SDL_PointInRect(&input->down_point, &hud->geometry)
                && !SDL_PointInRect(&input->down_point, &hud->tab_geometry)
            ) {
                return 1; // Not handled
            }
            break;
        default:
            return 1; // Not handled
    }

    if (input->type == INPUT_CLICK_UP)
    {
        if (SDL_PointInRect(&input->point, &hud->tab_geometry))
        {
            hud_toggle(hud);
        }
    }

    return 0;
}
