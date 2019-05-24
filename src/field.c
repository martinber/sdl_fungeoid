#include "field.h"

Field field_create(int width, int height, SDL_Point *screen_size, int cell_size)
{
    Field field =
    {
        .canvas = canvas_init(width, height),
        .screen_size = *screen_size,
        .ip = { 0, 0 },
        .speed = { 1, 0 },
        .cell_size = cell_size,
    };

    if (field.canvas.matrix == NULL) {
        return field;
        // TODO
    }
    canvas_set_instr(&field.canvas, 0, 0, INSTR_A);
    canvas_set_instr(&field.canvas, 1, 0, INSTR_B);
    canvas_set_instr(&field.canvas, 0, 1, INSTR_C);
    canvas_set_instr(&field.canvas, 9, 6, INSTR_D);
    canvas_set_instr(&field.canvas, 4, 13, INSTR_CHAROUT);
    canvas_set_instr(&field.canvas, 9, 13, INSTR_POP);
    return field;
}

void field_resize_screen(Field *field, SDL_Point *screen_size, int cell_size)
{
    field->screen_size = *screen_size;
    field->cell_size = cell_size;
}

int field_free(Field *field)
{
    return canvas_free(&field->canvas);
}

void field_handle_input(Field *field, Input *input)
{
    if (input->type == INPUT_CLICK_UP)
    {
        field->ip.x = input->point.x / field->cell_size;
        field->ip.y = input->point.y / field->cell_size;
    }
    else if (input->type == INPUT_CLICK_MOVE)
    {

    }
}

void field_handle_keyb(Field *field, KeyboardEvent *event)
{
    switch (event->type)
    {
        case KEYB_EVENT_ADD_INSTR:
            canvas_set_instr(&field->canvas, field->ip.x, field->ip.y, event->instr_id);
            break;
        case KEYB_EVENT_RM_INSTR:
            canvas_set_instr(&field->canvas, field->ip.x, field->ip.y, INSTR_SPACE);
            break;

        case KEYB_EVENT_MOVE_UP:
            field->ip.y -= 1;
            break;
        case KEYB_EVENT_MOVE_DOWN:
            field->ip.y += 1;
            break;
        case KEYB_EVENT_MOVE_LEFT:
            field->ip.x -= 1;
            break;
        case KEYB_EVENT_MOVE_RIGHT:
            field->ip.x += 1;
            break;

        default:
            break;
    }
}

void field_draw(SDL_Renderer *renderer, Field *field)
{
    int width = field->canvas.width;
    int height = field->canvas.height;
    int cell_size = field->cell_size;

    // Draw lines
    juan_set_render_draw_color(renderer, &COLOR_LINES);
    int line_width = cell_size / 20;

    for (int x = 0; x < width + 1; x++)
    {
        juan_draw_v_line_cap(renderer,
                x * cell_size, 0, height * cell_size, line_width);
    }
    for (int y = 0; y < height + 1; y++)
    {
        juan_draw_h_line_cap(renderer,
                0, y * cell_size, width * cell_size, line_width);
    }

    // Draw instructions
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            enum INSTR_ID id = canvas_get_instr(&field->canvas, x, y);
            if (id != INSTR_NULL && id != INSTR_SPACE)
            {
                SDL_Texture* tex = res_get_instr_tex(INSTR_THEME_BEFUNGE_CHAR, id);
                SDL_Rect r = {
                    x * cell_size,
                    y * cell_size,
                    cell_size,
                    cell_size
                };
                SDL_RenderCopy(renderer, tex, NULL, &r);
            }
        }
    }

    // Draw IP
    int ip_width = cell_size / 8;
    juan_set_render_draw_color(renderer, &COLOR_SELECT_1);
    juan_draw_h_line_cap(renderer,
            field->ip.x * cell_size,
            field->ip.y * cell_size,
            cell_size, ip_width);
    juan_draw_h_line_cap(renderer,
            field->ip.x * cell_size,
            (field->ip.y + 1) * cell_size,
            cell_size, ip_width);
    juan_draw_v_line_cap(renderer,
            field->ip.x * cell_size,
            field->ip.y * cell_size,
            cell_size, ip_width);
    juan_draw_v_line_cap(renderer,
            (field->ip.x + 1) * cell_size,
            field->ip.y * cell_size,
            cell_size, ip_width);
}
