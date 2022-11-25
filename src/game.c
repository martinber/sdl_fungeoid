#include "game.h"

#include "field.h"
#include "keyb.h"
#include "hud.h"
#include "os.h"

Game *game_create(SDL_Point window_size)
{
    Game *game = (Game*) malloc(sizeof(Game));
    if (game == NULL)
    {
        SDL_Log("Failed to malloc Game\n");
        return NULL;
    }


    Intrpr *intrpr = intrpr_create();
    Keyboard *keyb = keyb_create(window_size);
    Field *field = field_create(window_size, intrpr);
    Hud *hud = hud_create(window_size, 32, intrpr_get_stack(intrpr));
    if (field == NULL || keyb == NULL || hud == NULL)
    {
        game_free(game);
        game = NULL;

        SDL_Log("Failed to create field, keyb or hud");
        return NULL;
    }

    game->_intrpr = intrpr;
    game->_field = field;
    game->_keyb = keyb;
    game->_hud = hud;

    game->_filename[0] = '\0';
    game->_window_size = window_size;

    return game;
}

void game_free(Game *game)
{
    if (game != NULL)
    {
        game_autosave(game);

        if (game->_field != NULL)
        {
            field_free(game->_field);
            game->_field = NULL;
        }
        if (game->_keyb != NULL)
        {
            keyb_free(game->_keyb);
            game->_keyb = NULL;
        }
        if (game->_hud != NULL)
        {
            hud_free(game->_hud);
            game->_hud = NULL;
        }
    }
    free(game);
    game = NULL;
}

void game_update(Game *game, SDL_Renderer *renderer, Uint32 time_abs_ms)
{
    intrpr_update(game->_intrpr, time_abs_ms);
    field_update(game->_field, time_abs_ms);

    field_draw(renderer, game->_field);
    hud_draw(renderer, game->_hud);
    keyb_draw(renderer, game->_keyb);
}

void game_update_geometry(Game *game, SDL_Point window_size)
{
    game->_window_size = window_size;
    field_update_geometry(game->_field, game->_window_size);
    keyb_update_geometry(game->_keyb, game->_window_size);
    hud_update_geometry(game->_hud, game->_window_size, 32);
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
                game_load_filename_selected(game, event->drop.file + 5);
            }
            else if (!strncmp(event->drop.file, "saveas:", 7))
            {
                game_save_filename_selected(game, event->drop.file + 7);
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
    // First ask keyboard to handle input
    KeyboardEvent event = keyb_handle_input(game->_keyb, input);
    if (event.type == KEYB_EVENT_NOT_HANDLED)
    {
        // If not handled, ask hud to handle
        if (hud_handle_input(game->_hud, input) != 0)
        {
            // If neither of them handled, then it should be done by the field
            field_handle_input(game->_field, input);
        }
    }
    else
    {
        // If keyboard handled the input, process it or pass it to the field
        switch (event.type)
        {
            case KEYB_EVENT_LOAD:
                game_load_intent(game);
                break;

            case KEYB_EVENT_SAVE:
                game_save_intent(game);
                break;

            case KEYB_EVENT_SAVE_AS:
                game_save_as_intent(game);
                break;

            default:
                field_handle_keyb(game->_field, &event);
                break;
        }
    }
}

void game_new_intent(Game *game)
{
    // TODO
}

void game_load_intent(Game *game)
{
#ifdef __ANDROID__
    // If no file is selected, nothing happens. If a file is
    // selected, SDL will fire a SDL_DropEvent with
    // "open:/.../filename.bf"

    // This is a workaround because I don't know how to return from
    // this function which is actually implemented on Java.
    os_android_open_file_chooser();
#else
    // On GNU/Linux I receive the filename immediately
    {
        char buf[256] = "\0";
        os_linux_open_file_chooser(buf);
        if (strlen(buf) > 0)
        {
            game_load_filename_selected(game, buf);
        }
    }
#endif
}

void game_save_intent(Game *game)
{
    if (strlen(game->_filename) != 0)
    {
        game_save_filename_selected(game, game->_filename);
    }
    else
    {
        game_save_as_intent(game);
    }
}

void game_save_as_intent(Game *game)
{
#ifdef __ANDROID__
    // If no file is selected, nothing happens. If a file is
    // selected, SDL will fire a SDL_DropEvent with
    // "saveas:/.../filename.bf"

    // This is a workaround because I don't know how to return from
    // this function which is actually implemented on Java.
    os_android_save_file_as_chooser();
#else
    // On GNU/Linux I receive the filename immediately, I store it
    // on game->_filename and I save to it
    {
        char buf[256] = "\0";
        os_linux_save_file_as_chooser(buf);
        if (strlen(buf) > 0)
        {
            game_save_filename_selected(game, buf);
        }
    }
#endif
}

void game_save_filename_selected(Game *game, char *filename)
{
    if (game->_intrpr != NULL)
    {
        Canvas *canvas = intrpr_get_canvas(game->_intrpr);
        strcpy(game->_filename, filename);
        canvas_save(canvas, filename);
    } else {
        SDL_Log("Tried to save file when intrpr was NULL");
    }
}

void game_load_filename_selected(Game *game, char *filename)
{
    if (game->_intrpr != NULL)
    {
        Canvas *canvas = intrpr_get_canvas(game->_intrpr);
        strcpy(game->_filename, filename);
        canvas_load(canvas, filename);
    } else {
        SDL_Log("Tried to load file when intrpr was NULL");
    }
}

void game_new_user_sure(Game *game)
{
    // TODO
}

void game_autosave(Game *game)
{
    char buf[256] = "\0"; // TODO: use more than 256?
    if (os_get_autosave_file_path(buf) == 0)
    {
        if (game->_intrpr != NULL)
        {
            Canvas *canvas = intrpr_get_canvas(game->_intrpr);
            canvas_save(canvas, buf);
        } else {
            SDL_Log("Tried to save file when intrpr was NULL");
        }
    } else {
        SDL_Log("Failed to get autosave path");
    }
}
