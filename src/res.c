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
            char string[2] = { const_befunge_char(i), '\0' };
            INSTR_TEXTURES[i] = juan_text_texture(renderer, font_128, string, color);
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
