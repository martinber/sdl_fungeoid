#include "res.h"

#include "juan.h"

static TTF_Font* font_128 = NULL;

/// Array of textures for every instruction for the current thene and the current resolution
/**
 * Pointers for INSTR_NULL and INSTR_SPACE should be NULL
 */
static SDL_Texture *INSTR_TEXTURES[INSTR_ID_TOTAL] = { NULL };

/// Resolution of instruction textures (128x128)
static const int INSTR_TEXTURES_RES = 128;

static SDL_Texture *instr_tex_from_char(
    SDL_Renderer *renderer,
    TTF_Font *font,
    char character,
    SDL_Color color
) {
    // Create a empty square surface and draw a letter in the center of it
    // Apparently when rendering text on Blended mode, the surfaces are
    // ARGB32 bit

    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat
    (
        0,
        INSTR_TEXTURES_RES, INSTR_TEXTURES_RES,
        32, SDL_PIXELFORMAT_ARGB32
    );
    if (surface == NULL)
    {
        SDL_Log("Unable to create empty surface for char: %c\n", character);
        SDL_Log("SDL_CreateRGBSurfaceWithFormat Error: %s\n", SDL_GetError());
        return NULL;
    }

    char string[2] = { character, '\0' };
    SDL_Surface* text_surface = TTF_RenderText_Blended(font, string, color);
    if (text_surface == NULL)
    {
        SDL_FreeSurface(surface);
        SDL_Log("Unable to create surface for text: %s\n", string);
        SDL_Log("TTF_RenderText_Blended Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Rect dst_rect =
    {
        INSTR_TEXTURES_RES / 2 - text_surface->w / 2,
        0,
        0, // width ignored
        0, // height ignored
    };
    int blit_result = SDL_BlitSurface(text_surface, NULL, surface, &dst_rect);
    SDL_FreeSurface(text_surface);
    if (blit_result != 0)
    {
        SDL_FreeSurface(surface);
        SDL_Log("Unable to blit surface for char: %c\n", character);
        SDL_Log("SDL_BlitSurface Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == NULL)
    {
        SDL_Log("Unable to create texture for char: %c\n", character);
        SDL_Log("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        return NULL;
    }

    return texture;
}

int res_load_all(SDL_Renderer *renderer)
{
    /*
    RES_TEXTURES[RES_TEX_BALL] = juan_load_texture(renderer, "res/test.png");
    if (RES_TEXTURES[RES_TEX_BALL] == NULL) {
        // Remember to free succesfully loaded textures
        return 1;
    }
    */
    font_128 = juan_load_font("res/inconsolata/Inconsolata-Bold.ttf", 128);
    if (font_128 == NULL)
    {
        res_free_all();
        return 1;
    }

    // Load every instruction texture from characters

    SDL_Color color = { 255, 255, 255, 255 };
    for (enum INSTR_ID i = 0; i < INSTR_ID_TOTAL; i++)
    {
        if (i != INSTR_NULL && i != INSTR_SPACE)
        {
            INSTR_TEXTURES[i] = instr_tex_from_char(
                    renderer, font_128, const_befunge_char(i), color);
            if (INSTR_TEXTURES[i] == NULL)
            {
                SDL_Log("Error creating instruction texture for ID %d\n", i);
                res_free_all();
                return 1;
            }
        }
    }

    return 0;
}

void res_free_all()
{
    //SDL_DestroyTexture(RES_TEXTURES[RES_TEX_BALL]);
    //RES_TEXTURES[RES_TEX_BALL] = NULL;

    for (enum INSTR_ID i = 0; i < INSTR_ID_TOTAL; i++)
    {
        if (INSTR_TEXTURES[i] != NULL)
        {
            SDL_DestroyTexture(INSTR_TEXTURES[i]);
            INSTR_TEXTURES[i] = NULL;
        }
    }

    if (font_128 != NULL)
    {
        TTF_CloseFont(font_128);
        font_128 = NULL;
    }
}

SDL_Texture *res_get_instr_tex(enum INSTR_THEME theme, enum INSTR_ID id)
{
    UNUSED(theme);
    if (id == INSTR_NULL || id == INSTR_SPACE) {
        SDL_Log("Tried to get instruction texture of ID %d\n", id);
        return NULL;
    }
    return INSTR_TEXTURES[id];
}
