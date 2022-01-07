#include "game.h"

#include "field.h"
#include "keyb.h"
#include "hud.h"

Game *game_create(SDL_Point window_size)
{
    Game *game = (Game*) malloc(sizeof(Game));
    if (game == NULL)
    {
        SDL_Log("Failed to malloc Game\n");
        return NULL;
    }

    Field *field = field_create(200, 200, window_size);
    Keyboard *keyb = keyb_create(window_size);
    Hud *hud = hud_create(window_size, 32, field_get_stack(field));
    if (field == NULL || keyb == NULL || hud == NULL)
    {
        game_free(game);
        game = NULL;

        SDL_Log("Failed to create field, keyb or hud");
        return NULL;
    }

    game->_window_size = window_size;
    game->field = field;
    game->keyb = keyb;
    game->hud = hud;

    return game;
}

void game_free(Game *game)
{
    if (game != NULL)
    {
        if (game->field != NULL)
        {
            field_free(game->field);
            game->field = NULL;
        }
        if (game->keyb != NULL)
        {
            keyb_free(game->keyb);
            game->keyb = NULL;
        }
        if (game->hud != NULL)
        {
            hud_free(game->hud);
            game->hud = NULL;
        }
    }
    free(game);
    game = NULL;
}

void game_update(Game *game, SDL_Renderer *renderer, Uint32 time_abs_ms)
{
    field_update(game->field, time_abs_ms);

    field_draw(renderer, game->field);
    hud_draw(renderer, game->hud);
    keyb_draw(renderer, game->keyb);
}

void game_update_geometry(Game *game, SDL_Point window_size)
{
    game->_window_size = window_size;
    field_update_geometry(game->field, game->_window_size);
    keyb_update_geometry(game->keyb, game->_window_size);
    hud_update_geometry(game->hud, game->_window_size, 32);
}

void game_handle_event(Game *game, SDL_Event *event)
{
    switch (event->type)
    {
        case SDL_DROPFILE:
            // Skip 5 and 7 chars respectively because it starts with "open:" or
            // "saveas:"
            if (!strncmp(event->drop.file, "open:", 5))
            {
                field_load_filename_selected(game->field, event->drop.file + 5);
            }
            else if (!strncmp(event->drop.file, "saveas:", 7))
            {
                field_save_filename_selected(game->field, event->drop.file + 7);
            }
            else
            {
                SDL_Log("Received file drop event without prefix: %s", event->drop.file);
            }
            break;

        default:
            break;
    }
}

void game_handle_input(Game *game, Input *input)
{
    KeyboardEvent event = keyb_handle_input(game->keyb, input);
    if (event.type == KEYB_EVENT_NOT_HANDLED)
    {
        if (hud_handle_input(game->hud, input) != 0)
        {
            field_handle_input(game->field, input);
        }
    }
    else
    {
        field_handle_keyb(game->field, &event);
    }
}
