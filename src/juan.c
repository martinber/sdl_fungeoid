#include "juan.h"

int juan_init(
    SDL_Window **window,
    SDL_Renderer **renderer,
    const char *title,
    const int width,
    const int height
) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    int img_flags = IMG_INIT_PNG;
    if ((IMG_Init(img_flags) & img_flags) != img_flags)
    {
        printf("IMG_Init Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() != 0){
        printf("TTL_Init Error: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    *window = SDL_CreateWindow
    (
        title,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_SHOWN
    );
    if (*window == NULL)
    {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    *renderer = SDL_CreateRenderer
    (
        *window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (*renderer == NULL)
    {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    return 0;
}

SDL_Texture *juan_load_texture(SDL_Renderer *renderer, const char *path)
{
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    if (texture == NULL)
    {
        printf("Unable to load image: %s\n", path);
        printf("IMG_LoadTexture Error: %s\n", SDL_GetError());
    }
    return texture;
}

TTF_Font *juan_load_font(const char *path, int height)
{
    TTF_Font* font = TTF_OpenFont(path, height);
    if (font == NULL) {
        printf("Unable to load font: %s\n", path);
        printf("TTF_OpenFont Error: %s\n", SDL_GetError());
    }
    return font;
}

SDL_Texture *juan_text_texture(
    SDL_Renderer *renderer,
    TTF_Font *font,
    char *text,
    SDL_Color color
) {

    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    if (surface == NULL) {
        printf("Unable to create surface for text: %s\n", text);
        printf("TTF_RenderText_Blended Error: %s\n", SDL_GetError());
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == NULL) {
        printf("Unable to create texture for text: %s\n", text);
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    }

    return texture;
}
