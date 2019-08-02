#include "screens.h"

#include <time.h>

#include "input.h"
#include "field.h"
#include "keyb.h"
#include "hud.h"
#include "os.h"

ScreensHandler *screens_init(SDL_Point window_size)
{
    ScreensHandler *screens = (ScreensHandler*) malloc(sizeof(ScreensHandler));
    if (screens == NULL)
    {
        SDL_Log("Failed to malloc ScreensHandler\n");
        return NULL;
    }

    screens->current_screen = SCREEN_GAME;
    screens->input = input_create();
    screens->window_size = window_size;
    screens->loop_running = true;
    if (screens->input == NULL)
    {
        screens_free(screens);
        screens = NULL;

        SDL_Log("Error creating screens->input");
        return NULL;
    }

    // TODO remove
    int cell_size = juan_min(window_size.x / 15, window_size.y / 15);
    Field *field = field_create(10, 14, &window_size, cell_size);
    Keyboard *keyb = keyb_create(window_size);
    Hud *hud = hud_create(window_size, 32, field_get_stack(field));
    if (field == NULL || keyb == NULL || hud == NULL)
    {
        screens_free(screens);
        screens = NULL;

        SDL_Log("Failed to create field, keyb or InputHandler");
        return NULL;
    }

    screens->game_screen.field = field;
    screens->game_screen.keyb = keyb;
    screens->game_screen.hud = hud;

    return screens;
}

void screens_free(ScreensHandler *screens)
{
    if (screens != NULL)
    {
        if (screens->input != NULL)
        {
            input_free(screens->input);
            screens->input = NULL;
        }
        if (screens->game_screen.field != NULL)
        {
            field_free(screens->game_screen.field);
            screens->input = NULL;
        }
        if (screens->game_screen.keyb != NULL)
        {
            keyb_free(screens->game_screen.keyb);
            screens->input = NULL;
        }
        if (screens->game_screen.hud != NULL)
        {
            hud_free(screens->game_screen.hud);
            screens->input = NULL;
        }
    }
    free(screens);
    screens = NULL;
}

static void game_screen_handle_event(ScreensHandler *screens, SDL_Event *event)
{
    switch (event->type)
    {
        case SDL_KEYUP:
            if (event->key.keysym.sym == SDLK_AC_BACK)
            {
                screens->loop_running = false;
            }
            break;

        case SDL_DROPFILE:
            if (!strncmp(event->drop.file, "open:", 5))
            {
                field_load_filename_selected(screens->game_screen.field, event->drop.file + 5);
            }
            else if (!strncmp(event->drop.file, "saveas:", 7))
            {
                field_save_filename_selected(screens->game_screen.field, event->drop.file + 7);
            }
            else
            {
                SDL_Log("Received file drop event without prefix: %s", event->drop.file);
            }
            break;

        case SDL_WINDOWEVENT:
            if (event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                // TODO make these take references to the window size
                int cell_size = juan_min(screens->window_size.x / 15, screens->window_size.y / 15);
                field_resize_screen(screens->game_screen.field, &screens->window_size, cell_size);
                keyb_update_geometry(screens->game_screen.keyb, screens->window_size);
                hud_update_geometry(screens->game_screen.hud, screens->window_size, 32);
            }

        default:
            break;
    }
}

static void game_screen_handle_input(ScreensHandler *screens, Input *input)
{
    KeyboardEvent event = keyb_handle_input(screens->game_screen.keyb, input);
    if (event.type == KEYB_EVENT_NOT_HANDLED)
    {
        if (hud_handle_input(screens->game_screen.hud, input) != 0)
        {
            field_handle_input(screens->game_screen.field, input);
        }
    }
    else
    {
        field_handle_keyb(screens->game_screen.field, &event);
    }
}

static void game_screen_update
(
    ScreensHandler *screens,
    SDL_Renderer *renderer,
    Uint32 time_abs_ms
) {
    field_update(screens->game_screen.field, time_abs_ms);

    field_draw(renderer, screens->game_screen.field);
    hud_draw(renderer, screens->game_screen.hud);
    keyb_draw(renderer, screens->game_screen.keyb);
}

void screens_loop(ScreensHandler *screens, SDL_Renderer *renderer)
{

    // Time in milliseconds since start of the game
    Uint32 time_abs_ms = SDL_GetTicks();
    // Time in milliseconds since last loop
    Uint32 time_rel_ms = time_abs_ms;

    SDL_Event event;
    SDL_StartTextInput(); // So we get events of type SDL_TextInputEvent
    while (screens->loop_running)
    {
        time_rel_ms = SDL_GetTicks() - time_abs_ms;
        time_abs_ms = SDL_GetTicks();

        while (SDL_PollEvent(&event) != 0)
        {

            // Handle events for any screen

            switch (event.type)
            {
                case SDL_QUIT:
                    screens->loop_running = false;
                    break;

                case SDL_WINDOWEVENT:
                    screens->window_size.x = event.window.data1;
                    screens->window_size.y = event.window.data2;
                    break;

                default:
                    break;
            }

            // Handle events for each screen

            Input input_event = input_handle_event(screens->input, &screens->window_size, &event);

            switch (screens->current_screen)
            {
                case SCREEN_GAME:
                    game_screen_handle_event(screens, &event);
                    game_screen_handle_input(screens, &input_event);
                    break;

                default:
                    SDL_Log("Tried to handle event on invalid screen %d", screens->current_screen);
                    break;
            }
        }

        // Update and draw each screen

        juan_set_render_draw_color(renderer, &COLOR_BG);
        SDL_RenderClear(renderer);

        switch (screens->current_screen)
        {
            case SCREEN_GAME:
                game_screen_update(screens, renderer, time_abs_ms);
                break;

            default:
                SDL_Log("Tried to update invalid screen %d", screens->current_screen);
                break;

        }

        SDL_RenderPresent(renderer);
    }

}

