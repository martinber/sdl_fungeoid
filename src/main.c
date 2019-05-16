#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

/*
 * Functions return non-zero on error
 *
 * https://www.willusher.io/pages/sdl2/
 */

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 400;
const char WINDOW_TITLE[] = "sdl_fungeoid";

/*
 * Start SDL and open the window
 */
int init_sdl(SDL_Window **window, SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    *window = SDL_CreateWindow
    (
        WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (window == NULL)
    {
        printf("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    *renderer = SDL_CreateRenderer
    (
        *window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (renderer == NULL){
        SDL_DestroyWindow(*window);
        printf("SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    return 0;
}

void main_loop()
{
    bool running = true;

    while (running)
    {
    }
}


int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (init_sdl(&window, &renderer) != 0) {
        printf("Error creating window");
        return 1;
    }

    if (window == NULL) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        printf("Window Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    if (renderer == NULL) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        printf("Renderer Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface *bmp = SDL_LoadBMP("res/test.bmp");
    if (bmp == NULL)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        printf("SDL_LoadBMP Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, bmp);
    SDL_FreeSurface(bmp);
    if (tex == NULL)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        printf("SDL_CreateTextureFromSurface Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, tex, NULL, NULL);
    SDL_Delay(100);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);

    //main_loop();

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
