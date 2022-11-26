#include "text.h"

Text *text_create(int size, TTF_Font *font, SDL_Color color)
{
    Text *text = (Text*) malloc(sizeof(Text));
    if (text == NULL)
    {
        SDL_Log("Failed to malloc Text\n");
        return NULL;
    }

    text->_size = size;
    text->font = font;
    text->color = color;
    text->h_align = TEXT_ALIGN_START;
    text->v_align = TEXT_ALIGN_START;
    text->position = (SDL_Point) { 0, 0 };

    text->_str = (char*) malloc(sizeof(char) * size);
    if (text->_str == NULL)
    {
        text_free(text);
        text = NULL;

        SDL_Log("Failed to malloc text->_str\n");
        return NULL;
    }

    text->_texture = NULL;
    text_set_str(text, "");
    text->_update_texture = true;

    return text;
}

void text_free(Text *text)
{
    if (text != NULL)
    {
        SDL_DestroyTexture(text->_texture);
        text->_texture = NULL;

        free(text->_str);
        text->_str = NULL;
    }
    free(text);
    text = NULL;

}

void text_set_str(Text *text, char *str)
{
    if (strcmp(text->_str, str) != 0) {
        strcpy(text->_str, str);
        text->_update_texture = true;
    }
}

void text_draw(SDL_Renderer *renderer, Text *text)
{
    if (text->_update_texture)
    {
        text->_texture = juan_text_texture(
            renderer,
            text->font,
            text->_str,
            text->color
        );
        if (text->_texture == NULL)
        {
            SDL_Log("Error rendering text: %s", text->_str);
            return;
        }
        text->_update_texture = false;
    }

    SDL_Rect geometry;
    SDL_QueryTexture(text->_texture, NULL, NULL, &geometry.w, &geometry.h);

    switch (text->h_align)
    {
        case TEXT_ALIGN_START:
            geometry.x = text->position.x;
            break;
        case TEXT_ALIGN_END:
            geometry.x = text->position.x - geometry.w;
            break;
        case TEXT_ALIGN_CENTER:
            geometry.x = text->position.x - geometry.w / 2;
            break;
    }

    switch (text->v_align)
    {
        case TEXT_ALIGN_START:
            geometry.y = text->position.y;
            break;
        case TEXT_ALIGN_END:
            geometry.y = text->position.y - geometry.h;
            break;
        case TEXT_ALIGN_CENTER:
            geometry.y = text->position.y - geometry.h / 2;
            break;
    }

    SDL_RenderCopy(renderer, text->_texture, NULL, &geometry);
}
