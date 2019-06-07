#include "input.h"

InputHandler *input_create()
{
    InputHandler *handler = (InputHandler*) malloc(sizeof(InputHandler));
    if (handler == NULL)
    {
        SDL_Log("Failed to malloc InputHandler");
        return NULL;
    }

    handler->click_candidate = INPUT_NONE;
    handler->last_touch_id = 0;
    handler->down_point = (SDL_Point) { 0, 0 };
    handler->sq_threshold = 40*40;

    return handler;
}

void input_free(InputHandler *handler)
{
    free(handler);
    handler = NULL;
}

Input input_handle_event(InputHandler *handler, SDL_Point *window_size, SDL_Event *event)
{
    Input input =
    {
        .type = INPUT_NONE,
        .point = { 0, 0 },
        .down_point = { 0, 0 },
        .diff = { 0, 0 },
        .dist = { 0, 0 },
        .key = 0,
    };

    switch (event->type)
    {
        case SDL_KEYDOWN:
            input.type = INPUT_KEY_DOWN;
            input.key = event->key.keysym.sym;
            break;

        case SDL_KEYUP:
            input.type = INPUT_KEY_UP;
            input.key = event->key.keysym.sym;
            break;

        case SDL_FINGERDOWN:
            if (handler->click_candidate == INPUT_NONE)
            {
                input.point.x = event->tfinger.x * window_size->x;
                input.point.y = event->tfinger.y * window_size->y;

                handler->click_candidate = INPUT_CLICK_UP;
                handler->last_touch_id = event->tfinger.fingerId;
                handler->down_point = input.point;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (handler->click_candidate == INPUT_NONE)
            {
                input.point.x = event->button.x;
                input.point.y = event->button.y;

                handler->click_candidate = INPUT_CLICK_UP;
                handler->last_touch_id = 0;
                handler->down_point = input.point;
            }
            break;

        case SDL_FINGERUP:
            input.type = handler->click_candidate; // Can be INPUT_NONE maybe?
            input.point.x = event->tfinger.x * window_size->x;
            input.point.y = event->tfinger.y * window_size->y;

            if (handler->click_candidate == INPUT_CLICK_MOVE_UP)
            {
                input.dist = juan_point_sub(&input.point, &handler->down_point);
                input.down_point = handler->down_point;
            }

            handler->click_candidate = INPUT_NONE;
            break;

        case SDL_MOUSEBUTTONUP:
            input.type = handler->click_candidate; // Can be INPUT_NONE maybe?
            input.point.x = event->button.x;
            input.point.y = event->button.y;

            if (handler->click_candidate == INPUT_CLICK_MOVE_UP)
            {
                input.dist = juan_point_sub(&input.point, &handler->down_point);
                input.down_point = handler->down_point;
            }

            handler->click_candidate = INPUT_NONE;
            break;

        case SDL_FINGERMOTION:
            input.point.x = event->tfinger.x * window_size->x;
            input.point.y = event->tfinger.y * window_size->y;

            if (handler->click_candidate == INPUT_CLICK_UP)
            {
                if (juan_ftoi(juan_point_sq_dist(&input.point, &handler->down_point))
                        > handler->sq_threshold
                ) {
                    handler->click_candidate = INPUT_CLICK_MOVE_UP;
                }
            }
            if (handler->click_candidate == INPUT_CLICK_MOVE_UP)
            {
                input.type = INPUT_CLICK_MOVE;
                input.dist = juan_point_sub(&input.point, &handler->down_point);
                input.down_point = handler->down_point;
                input.diff.x = event->tfinger.dx * window_size->x;
                input.diff.y = event->tfinger.dy * window_size->y;
            }
            break;

        case SDL_MOUSEMOTION:
            // Event received even if there is no pressed button
            input.point.x = event->button.x;
            input.point.y = event->button.y;

            if (handler->click_candidate == INPUT_CLICK_UP)
            {
                if (juan_ftoi(juan_point_sq_dist(&input.point, &handler->down_point))
                        > handler->sq_threshold
                ) {
                    handler->click_candidate = INPUT_CLICK_MOVE_UP;
                }
            }
            if (handler->click_candidate == INPUT_CLICK_MOVE_UP)
            {
                input.type = INPUT_CLICK_MOVE;
                input.dist = juan_point_sub(&input.point, &handler->down_point);
                input.down_point = handler->down_point;
                input.diff.x = event->motion.xrel;
                input.diff.y = event->motion.yrel;
            }
            break;

        case SDL_TEXTINPUT:
            input.type = INPUT_TEXT;
            strncpy(input.text, event->text.text, 32);
            input.text[31] = '\0'; // Just in case
            break;
    }

    return input;
}
