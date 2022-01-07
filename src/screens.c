#include "screens.h"

#include <time.h>

#include "input.h"
#include "os.h"

ScreensHandler *screens_create(SDL_Point window_size)
{
    ScreensHandler *screens = (ScreensHandler*) malloc(sizeof(ScreensHandler));
    if (screens == NULL)
    {
        SDL_Log("Failed to malloc ScreensHandler\n");
        return NULL;
    }

    screens->_current_screen = SCREEN_GAME;
    screens->window_size = window_size;
    screens->_loop_running = true;

    screens->_input = input_create();
    screens->_game = game_create(window_size);
    if (screens->_input == NULL || screens->_game == NULL)
    {
        screens_free(screens);
        screens = NULL;

        SDL_Log("Error creating screens->_input or screens->_game");
        return NULL;
    }

    return screens;
}

void screens_free(ScreensHandler *screens)
{
    if (screens != NULL)
    {
        if (screens->_input != NULL)
        {
            input_free(screens->_input);
            screens->_input = NULL;
        }
        if (screens->_game != NULL)
        {
            game_free(screens->_game);
            screens->_game = NULL;
        }
    }
    free(screens);
    screens = NULL;
}

void screens_loop(ScreensHandler *screens, SDL_Renderer *renderer)
{

    // Time in milliseconds since start of the game
    Uint32 time_abs_ms = SDL_GetTicks();

    SDL_Event event;
    SDL_StartTextInput(); // So we get events of type SDL_TextInputEvent
    while (screens->_loop_running)
    {
        time_abs_ms = SDL_GetTicks();

        while (SDL_PollEvent(&event) != 0)
        {

            // Handle events for any screen

            switch (event.type)
            {
                case SDL_QUIT:
                    screens->_loop_running = false;
                    break;

                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED
                            || event.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        screens->window_size.x = event.window.data1;
                        screens->window_size.y = event.window.data2;
                        game_update_geometry(screens->_game, screens->window_size);
                    }
                    break;

                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_AC_BACK)
                    {
                        screens->_loop_running = false;
                    }
                    break;

                default:
                    break;
            }

            // Handle events for each screen

            Input input_event = input_handle_event(screens->_input, &screens->window_size, &event);

            switch (screens->_current_screen)
            {
                case SCREEN_GAME:
                    game_handle_event(screens->_game, &event);
                    game_handle_input(screens->_game, &input_event);
                    break;

                default:
                    SDL_Log("Tried to handle event on invalid screen %d", screens->_current_screen);
                    break;
            }
        }

        // Update and draw each screen

        juan_set_render_draw_color(renderer, &COLOR_BG);
        SDL_RenderClear(renderer);

        switch (screens->_current_screen)
        {
            case SCREEN_GAME:
                game_update(screens->_game, renderer, time_abs_ms);
                break;

            default:
                SDL_Log("Tried to update invalid screen %d", screens->_current_screen);
                break;

        }

        SDL_RenderPresent(renderer);
    }

}

