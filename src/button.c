#include "button.h"

void button_init(Button *button, enum BUTTON_TYPE type, int id)
{
    button->geometry = (SDL_Rect) { 0, 0, 0, 0 };
    button->toggled = false;
    button->pressed = false;
    button->type = type;
    button->id = id;
}
